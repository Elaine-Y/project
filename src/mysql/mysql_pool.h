//
// Created by Elaine on 2022/2/12.
// Here is Mysql Pool
//
#pragma once

#include <assert.h>
#include <mysql/mysql.h>

#include <condition_variable>
#include <iostream>
#include <list>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

using std::list;
using std::string;

// 返回结果 select的时候用
class ResultSet {
 public:
  ResultSet() {}
  ResultSet(MYSQL_RES* res);
  virtual ~ResultSet();

  bool next();
  int32_t get_int(const string& key);
  string get_string(const string& key);

 private:
  int32_t get_index(const string& key);

  MYSQL_RES* m_res;
  MYSQL_ROW m_row;
  std::map<string, uint32_t> m_key_map;
};

class MysqlPool;

class MysqlConn {
 public:
  MysqlConn(MysqlPool* db_pool);
  virtual ~MysqlConn();
  bool init();

  // 查询
  ResultSet* do_query(const string& sql);
  // 获取连接池名
  const string get_pool_name();
  MYSQL* get_mysql() { return m_mysql; }

 private:
  MysqlPool* m_dbpool;  // to get MySQL server information
  MYSQL* m_mysql;       // 对应一个连接
};

// 只是负责管理连接DBConn，真正干活的是DBConn
class MysqlPool {
 public:
  static MysqlPool* get_mysql_object();  //单例模式
  void set_parameters(const string pool_name, const string db_server_ip,
                      const uint16_t db_server_port, const string username,
                      const string password, const string db_name,
                      uint32_t max_conn_cnt);
  virtual ~MysqlPool();

  bool init();  // 连接数据库，创建连接
  MysqlConn* get_db_conn(const int32_t timeout_ms = 0);  // 获取连接资源
  void release_db_conn(MysqlConn* p_conn);               // 归还连接资源

  const char* get_pool_name() { return m_pool_name.c_str(); }
  const char* get_db_server_ip() { return m_db_server_ip.c_str(); }
  uint16_t get_db_server_port() { return m_db_server_port; }
  const char* get_user_name() { return m_username.c_str(); }
  const char* get_password() { return m_password.c_str(); }
  const char* get_db_name() { return m_db_name.c_str(); }

 private:
  MysqlPool() {}
  static MysqlPool* m_mysql_object;  //单例模式对象

  string m_pool_name;                 // 连接池名称
  string m_db_server_ip;              // 数据库ip
  uint16_t m_db_server_port;          // 数据库端口
  string m_username;                  // 用户名
  string m_password;                  // 用户密码
  string m_db_name;                   // db名称
  uint32_t m_db_cur_conn_cnt;         // 当前启用的连接数量
  uint32_t m_db_max_conn_cnt;         // 最大连接数量
  std::list<MysqlConn*> m_free_list;  // 空闲的连接

  std::list<MysqlConn*> m_used_list;  // 记录已经被请求的连接
  std::mutex m_mutex;
  static std::mutex m_object_mutex;  //对象锁
  std::condition_variable m_cond_var;
  bool m_abort_request = false;
};

// SQL拼接 防注入
class SqlConcat {
 public:
  class Split {
    friend class SqlConcat;

   public:
    Split(const string& sql_str = "") {
      if (!sql_str.empty()) {
        split_str(sql_str, m_seg, "?");
      }
    }

   private:
    std::vector<std::string> m_seg;
    void split_str(const string& str, std::vector<std::string>& ret,
                   const std::string& sep = ",") {
      int start = 0;
      int end = str.find(sep);
      string tmp;
      while (end != -1) {
        ret.push_back(str.substr(start, end - start));
        start = end + sep.size();
        end = str.find(sep, start);
      }
      ret.push_back(str.substr(start, end - start));
    }
  };

  SqlConcat(std::ostringstream& ss, const Split& shared_split)
      : m_ss(ss), m_seg(shared_split.m_seg), m_idx(0) {
    ++*this;
  }

  ~SqlConcat() { check_range(); }

  inline std::ostringstream& operator*() { return m_ss; }
  inline std::ostringstream* operator->() { return &m_ss; }

  inline void check_range() { assert(m_idx == m_seg.size()); }

  template <typename T>
  inline SqlConcat& operator<<(const T& arg) {
    m_ss << arg;
    return ++*this;
  }
  inline SqlConcat& operator<<(const std::string& str) {
    m_ss << '\'' << str << '\'';
    return ++*this;
  }
  inline SqlConcat& operator<<(const char* str) {
    m_ss << '\'' << str << '\'';
    return ++*this;
  }
  inline SqlConcat& operator<<(char* str) {
    m_ss << '\'' << str << '\'';
    return ++*this;
  }
  inline SqlConcat& operator<<(const void* str) {
    m_ss << (const char*)str;
    return ++*this;
  }

  inline SqlConcat& operator++() {
    m_ss << m_seg[m_idx++];
    return *this;
  }

  std::ostringstream& m_ss;
  const std::vector<std::string>& m_seg;
  uint32_t m_idx;
};