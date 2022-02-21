//
// Created by Elaine on 2022/2/12.
// Here is QueryInfo
//
#include "query_info.h"

QueryInfo::QueryInfo(Hello::AsyncService* service,
                     ServerCompletionQueue* completion_queue, MysqlPool* db)
    : m_service(service),
      m_completion_query(completion_queue),
      m_responder(&m_ctx),
      m_db(db), m_status(CREATE) {
  query_proceed();
}

void QueryInfo::query_proceed() {
  if (m_status == CREATE) {
    // create
    m_status = PROCESS;
    m_service->RequestGetKey(&m_ctx, &m_request, &m_responder,
                             m_completion_query, m_completion_query, this);
    std::cout << "[INFO] server: receive from client." << std::endl;
  } else if (m_status == PROCESS) {
    // proceed
    m_status = FINISH;
    new QueryInfo(m_service, m_completion_query, m_db);

    Status ret = Status::OK;
    do {
      if (!&m_request) {
        ret = Status(grpc::INVALID_ARGUMENT, "INVALID_REQUEST");
      }
      if (m_request.out_trade_no().empty()) {
        ret = Status(grpc::INVALID_ARGUMENT, "PARAM_ERROR");
      }
      if (m_request.mchid().empty()) {
        ret = Status(grpc::INVALID_ARGUMENT, "PARAM_ERROR");
      }
      std::cout << "[DEBUG] req:" << m_request.ShortDebugString() << std::endl;
      ret = deal_msg(m_request, &m_response);

    } while (false);
    m_responder.Finish(m_response, ret, this);
    std::cout << "server: done for client" << std::endl;

  } else {
    GPR_ASSERT(m_status == FINISH);
    // finish
    std::cout << "delete a query_info" << std::endl;
    delete this;
  }
}

Status QueryInfo::deal_msg(const Request& req, Response* rsp) {
  if (!m_db->init()) {
    std::cout << "[ERR] mysql pool init failed." << std::endl;
  }
  MysqlConn* db_conn = m_db->get_db_conn(2000);
  if (nullptr == db_conn) {
    std::cout << "[ERR] null db handler" << std::endl;
    rsp_err(*rsp, 1, std::string("get db conn failed"));
    return Status(grpc::UNKNOWN, "SYSTEMERROR");
  }

  std::string sql_t =
      "select appid from business_order where mchid=" + req.mchid() +
      " and "
      "out_trade_no=" +
      req.out_trade_no();

  ResultSet* res = db_conn->do_query(sql_t);

  // 数据唯一
  std::string appid;
  while (res->next()) {
    appid = res->get_string("appid");
    std::cout << "[Test] get appid from DB:" << appid << std::endl;
    rsp->set_appid(appid);
  }
  return Status(grpc::OK, "");
}

void QueryInfo::rsp_err(Response& rsp, int err_code,
                        const std::string& err_msg) {
  rsp.set_err_code(err_code);
  rsp.set_err_msg(err_msg);
}