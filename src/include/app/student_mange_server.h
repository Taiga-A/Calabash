//
// Created by Taiga on 2023/3/23.
//

#ifndef CALABASH_SRC_INCLUDE_APP_STUDENT_MANGE_SERVER_H_
#define CALABASH_SRC_INCLUDE_APP_STUDENT_MANGE_SERVER_H_

#include "thread"
#include "memory"
#include "chrono"

#include "web/server.h"

//#define DB_DROP_TABLES_AT_START

namespace calabash {

class Sqlite3;

constexpr char kTokenKey[] = "calabash_token_password";

class StudentMangeServer {
 private:
  StudentMangeServer() = default;
  ~StudentMangeServer() = default;

 public:
  static inline StudentMangeServer *Instance();

  void Init(int port = 8080, const std::string& db_name = "calabash.db");

  void Start();
  [[maybe_unused]] void Stop();
  [[nodiscard]] inline bool Status() const { return is_start_; };

 private:
  void DataBaseInit();

  void app_login(const Request &req, Response &res);
  void app_self(const Request &req, Response &res);

 private:
  bool is_init_ = false;
  bool is_start_ = false;
  std::shared_ptr<std::thread> server_thread_;
  std::shared_ptr<Server> http_server_;
  std::shared_ptr<Sqlite3> db_;
};

StudentMangeServer *StudentMangeServer::Instance() {
  static StudentMangeServer *p = nullptr;
  if (p == nullptr) {
    p = new StudentMangeServer();
  }
  return p;
}


} // namespace calabash

#endif //CALABASH_SRC_INCLUDE_APP_STUDENT_MANGE_SERVER_H_
