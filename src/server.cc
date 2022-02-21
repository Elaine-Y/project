//
// Created by Elaine on 2022/2/12.
// Here is Server
//
// internal stl
#include <unistd.h>

// lib outside
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

// user lib
#include "mysql/mysql_pool.h"
#include "proto/hello.grpc.pb.h"
#include "query_info.h"
#include "utils/utils.h"

using std::string;
using std::unique_ptr;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerWriter;

class ServerImpl final {
 public:
  ServerImpl(){};
  ~ServerImpl() {
    m_server->Shutdown();
    m_completion_query->Shutdown();
  }
  void run(const string& server_address, MysqlPool* db_pool,
           const int& max_reiceive_message_size = 5000,
           const int& max_send_message_size = 5000, const int& thread_cnt = 2) {
    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    m_db = db_pool;

    // init a builder
    ServerBuilder builder;
    // set builder address and credential, todo secure
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // register service to build
    builder.RegisterService(&m_service);
    // set max receive and send message
    builder.SetMaxReceiveMessageSize(max_reiceive_message_size);
    builder.SetMaxSendMessageSize(max_send_message_size);

    m_completion_query = builder.AddCompletionQueue();
    m_server = builder.BuildAndStart();
    std::cout << "[INFO] Server listening on " << server_address << std::endl;

    polling();
  }

  //轮询
  void polling() {
    new QueryInfo(&m_service, m_completion_query.get(), m_db);
    std::cout << "create a new query info" << std::endl;
    void* tag;
    bool ok;
    while (true) {
      GPR_ASSERT(m_completion_query->Next(&tag, &ok));
      GPR_ASSERT(ok);
      static_cast<QueryInfo*>(tag)->query_proceed();
    }
  };

 private:
  unique_ptr<ServerCompletionQueue> m_completion_query;
  unique_ptr<Server> m_server;
  Hello::AsyncService m_service;
  MysqlPool* m_db;
};

void load_parameters(MysqlPool* db_pool) {
  // DB config
  const string& pool_name = "mysql_pool";
  const string& db_server_ip = "127.0.0.1";
  const uint16_t db_server_port = 3306;
  const string& username = "root";
  const string& password = "123456";
  const string& db_name = "demo";
  uint32_t max_conn_cnt = 2;  // todo

  db_pool->set_parameters(pool_name, db_server_ip, db_server_port, username,
                          password, db_name, max_conn_cnt);
}

int main(int argc, char* argv[]) {
  MysqlPool* db_pool = MysqlPool::get_mysql_object();
  load_parameters(db_pool);
  if (!db_pool->init()) {
    std::cout << "db init failed." << std::endl;
    return -1;
  }

  const string server_address("0.0.0.0:50001");
  ServerImpl server;
  server.run(server_address, db_pool);

  delete db_pool;
  return 0;
}
