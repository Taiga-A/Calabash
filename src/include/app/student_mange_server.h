//
// Created by Taiga on 2023/3/23.
//

#ifndef CALABASH_SRC_INCLUDE_APP_STUDENT_MANGE_SERVER_H_
#define CALABASH_SRC_INCLUDE_APP_STUDENT_MANGE_SERVER_H_

#include "thread"
#include "memory"

#include "database/sqlite3.h"
#include "web/server.h"

namespace calabash {

class StudentMangeServer {
 private:
  StudentMangeServer() = default;
  ~StudentMangeServer() = default;

 public:
  static inline StudentMangeServer *Instance();

  void Init(int port = 8080, const std::string& db_name = "calabash.db");

  void Start();
  void Stop();
  [[nodiscard]] inline bool Status() const { return is_start_; };

 private:
  void DataBaseInit();

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
