//
// Created by Taiga on 2023/3/23.
//

#ifndef CALABASH_SRC_INCLUDE_APP_STUDENT_MANGE_SERVER_H_
#define CALABASH_SRC_INCLUDE_APP_STUDENT_MANGE_SERVER_H_

#include "thread"
#include "memory"
#include "chrono"
#include "functional"

#include "web/server.h"
#include "web/exception.h"

// #define DB_DROP_TABLES_AT_START

namespace calabash {

class Sqlite3;

constexpr char kTokenKey[] = "calabash_token_password";

class StudentMangeServer {
 private:
  StudentMangeServer() = default;

  ~StudentMangeServer() = default;

 public:
  class StudentStatus {
   public:
    enum LeaveStatus {
      PendingApproval = 0x00,  // wait
      Passed = 0x01,           // pass
      Rejection = 0x02,        // not pass
    };
    StudentStatus() = default;
    explicit StudentStatus(int status);
    [[nodiscard]] inline int status() const { return instructor_ | (mentor_ << 2) | (is_destroyed_ << 4); };
    [[nodiscard]] inline bool is_passed() const { return !is_destroyed_ && mentor_ == Passed && instructor_ == Passed;}
    LeaveStatus instructor_ = PendingApproval;
    union {
      LeaveStatus mentor_ = PendingApproval;
      bool is_junior_;
    };
    bool is_destroyed_ = false;
  };

  static inline StudentMangeServer *Instance();

  void Init(int port = 8080, const std::string &db_name = "calabash.db");

  void Start();

  [[maybe_unused]] void Stop();

  [[nodiscard]] inline bool Status() const { return is_start_; };

 private:
  void DataBaseInit();

  // applications
  static void app_process_exception(Request &req, Response &res, const ServerNextFunc &next_func);

  static void app_token_parse(Request &req, Response &res, const ServerNextFunc &next_func);

  void app_login(const Request &req, Response &res, const ServerNextFunc &next_func);

  void app_self(const Request &req, Response &res, const ServerNextFunc &next_func);

  void app_leave(const Request &req, Response &res, const ServerNextFunc &next_func);

  void app_release(const Request &req, Response &res, const ServerNextFunc &next_func);

  void app_student_info(const Request &req, Response &res, const ServerNextFunc &next_func);

  void app_search_leave(const Request &req, Response &res, const ServerNextFunc &next_func);

  void app_search_leave_num(const Request &req, Response &res, const ServerNextFunc &next_func);

  void app_approval(const Request &req, Response &res, const ServerNextFunc &next_func);

  static void TestJsonParam(const nlohmann::json &j, std::initializer_list<std::string> params);

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
