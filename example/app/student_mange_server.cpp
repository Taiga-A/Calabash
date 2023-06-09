//
// Created by Taiga on 2023/3/23.
//

#include "app/student_mange_server.h"
#include "database/sqlite3.h"

using namespace calabash;
using namespace std;

void StudentMangeServer::Start() {
  ASSERT(is_init_)
  if (is_start_) {
    WARNING("http server have been started!");
    return;
  }
  this->server_thread_ = make_shared<thread>([&](){
    SYSTEM("Http server start");
    http_server_->Start();
    SYSTEM("Http server stop");
  });
  is_start_ = true;
}

[[maybe_unused]] void StudentMangeServer::Stop() {
  if (!is_start_) {
    WARNING("http server have been stop!");
    return;
  }
  http_server_->Stop();
  server_thread_->join();
  server_thread_ = nullptr;
  is_start_ = false;
}

#define MEMBER_FUN_BIND(func)                              \
  [this](auto && PH1, auto && PH2, auto && NEXT) {         \
    func(std::forward<decltype(PH1)>(PH1),                 \
        std::forward<decltype(PH2)>(PH2),                  \
        std::forward<decltype(NEXT)>(NEXT));               \
    }

void StudentMangeServer::Init(int port, const std::string &db_name) {
  db_ = make_shared<Sqlite3>(db_name);
  DataBaseInit();
  http_server_ = make_shared<Server>();
  http_server_->Listen("", port);
  http_server_->StaticPath("./static");
  // server init  [👇]  ↓
  http_server_->ProcessBind(app_process_exception);
  http_server_->Bind("/login", {MEMBER_FUN_BIND(app_login)});
  http_server_->Bind("/self", {app_token_parse, MEMBER_FUN_BIND(app_self)});
  http_server_->Bind("/leave", {app_token_parse, MEMBER_FUN_BIND(app_leave)});
  http_server_->Bind("/release", {app_token_parse, MEMBER_FUN_BIND(app_release)});
  http_server_->Bind("/student_info", {app_token_parse, MEMBER_FUN_BIND(app_student_info)});
  http_server_->Bind("/search_leave", {app_token_parse, MEMBER_FUN_BIND(app_search_leave)});
  http_server_->Bind("/search_leave_num", {app_token_parse, MEMBER_FUN_BIND(app_search_leave_num)});
  http_server_->Bind("/approval", {app_token_parse, MEMBER_FUN_BIND(app_approval)});

  is_init_ = true;
}

#undef MEMBER_FUN_BIND

void StudentMangeServer::DataBaseInit() {
#ifdef DB_DROP_TABLES_AT_START
  SYSTEM("Defined DB_DROP_TABLES_AT_START");
  SYSTEM("Drop TABLE IF EXISTS students");
  db_->SQL("DROP TABLE students");
  SYSTEM("Drop TABLE IF EXISTS teachers");
  db_->SQL("DROP TABLE teachers");
  SYSTEM("Drop TABLE IF EXISTS leave_info");
  db_->SQL("DROP TABLE leave_info");
#endif

  SYSTEM("Load table students");
  db_->SQL(
#include "inc/DBTable_students_SQL.inc"
      );
  SYSTEM("Load table teachers");
  db_->SQL(
#include "inc/DBTable_teachers_SQL.inc"
      );
  SYSTEM("Load table leave_info");
  db_->SQL(
#include "inc/DBTable_leave_info_SQL.inc"
      );
  db_->BindCompiledSQL("insert_student",
       "INSERT INTO students(id, name, specialty, class, sex, tel, password, instructor_id, mentor_id)"
       "VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9)");
  db_->BindCompiledSQL("insert_teacher",
       "INSERT INTO teachers(id, name, sex, age, tel, password)"
       "VALUES (?1, ?2, ?3, ?4, ?5, ?6)");
  db_->BindCompiledSQL("insert_leave_info",
       "INSERT INTO leave_info(student_id, mentor_id, instructor_id, leave_type, leave_reason, time_begin, time_end, is_school, status)"
       "VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9)");
  db_->BindCompiledSQL("student_info",
       "SELECT * FROM students WHERE id = ?1");
  db_->BindCompiledSQL("student_password",
       "SELECT password FROM students WHERE id = ?1");
  db_->BindCompiledSQL("teacher_info",
       "SELECT * FROM teachers WHERE id = ?1");
  db_->BindCompiledSQL("teacher_password",
       "SELECT password FROM teachers WHERE id = ?1");
  db_->BindCompiledSQL("select_leave_status",
       "SELECT status FROM leave_info WHERE inform_id = ?1");
  db_->BindCompiledSQL("select_leave_status_teachers",
       "SELECT status, instructor_id, mentor_id FROM leave_info WHERE inform_id = ?1");
  db_->BindCompiledSQL("update_leave_info_status",
       "UPDATE leave_info SET status = ?2 WHERE inform_id = ?1");
  db_->BindCompiledSQL("select_leave_info_instructor",
       "SELECT * FROM leave_info WHERE instructor_id = ?1");
  db_->BindCompiledSQL("select_leave_info_mentor",
       "SELECT * FROM leave_info WHERE mentor_id = ?1");
  db_->BindCompiledSQL("select_leave_info_student",
       "SELECT * FROM leave_info WHERE student_id = ?1");
}

void StudentMangeServer::TestJsonParam(const nlohmann::json &j, std::initializer_list<std::string> params) {
  string lost_params;
  for (auto & str : params) {
    if (j.find(str) == j.end())
      lost_params += ", " + str;
  }
  if (!lost_params.empty())
    throw HttpException(403, "Lost param : " + string(lost_params.c_str() + 2));
}

StudentMangeServer::StudentStatus::StudentStatus(int status) {
  this->is_destroyed_ = 0x01 & (status >> 4);
  this->instructor_ = static_cast<LeaveStatus>(0x03 & status);
  this->mentor_ = static_cast<LeaveStatus>(0x03 & (status >> 2));
}

