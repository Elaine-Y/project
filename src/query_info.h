//
// Created by Elaine on 2022/2/12.
// Here is QueryInfo
//
#pragma once

// stl lib
// system lib
// user lib
#include "mysql/mysql_pool.h"
#include "proto/hello.grpc.pb.h"

using grpc::ServerAsyncResponseWriter;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;
using helloworld::Hello;
using helloworld::Request;
using helloworld::Response;

class QueryInfo {
 public:
  QueryInfo(Hello::AsyncService* service,
            ServerCompletionQueue* completion_queue, MysqlPool* db);
  void query_proceed();

 private:
  Hello::AsyncService* m_service;
  ServerCompletionQueue* m_completion_query;
  MysqlPool* m_db;
  ServerContext m_ctx;
  Request m_request;
  Response m_response;
  ServerAsyncResponseWriter<Response> m_responder;

  // read write lock
  //  pthread_rwlock_t* m_rwlock;

  Status deal_msg(const Request& req, Response* rsp);
  void rsp_err(Response& rsp, int err_code, const std::string& err_msg);

  enum CallStatus { CREATE, PROCESS, FINISH };
  CallStatus m_status;  // The current serving state.
};