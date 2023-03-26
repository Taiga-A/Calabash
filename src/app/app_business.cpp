//
// Created by Taiga on 2023/3/25.
//

#include "nlohmann/json.hpp"

#include <chrono>

#include "app/student_mange_server.h"
#include "app/token.h"
#include "database/sqlite3.h"

using namespace nlohmann;
using namespace calabash;
using namespace std;

void StudentMangeServer::app_process_exception(Request &req,Response &res,const NextFunc &next_func) {
  try {
    if (req.method() != "POST")
      throw HttpException(405, "Method Not Allowed, you must use POST method");
    if (req.is_json()) {
      req.json() = json::parse(req.body());
    }
    next_func();
    res.set_code(200);
    json res_json = {
        {"code", 200},
        {"msg", "ok"}
    };
    res.SetJsonType();
    res.BodyData(res_json.dump(2));
  } catch (const HttpException &e) {
    res.set_code(e.base_code());
    json res_json = e.body();
    res_json["code"] = e.code();
    res_json["msg"] = e.msg();
    res.SetJsonType();
    res.BodyData(res_json.dump(2));
  } catch (const json::exception &e) {
    res.set_code(403);
    json res_json = {
        {"code", 403},
        {"msg", e.what()}
    };
    res.SetJsonType();
    res.BodyData(res_json.dump(2));
  } catch (const exception &e) {
    cout << "er:" << e.what() <<  endl;
    res.set_code(300);
    json res_json = {
        {"code", 300},
        {"msg", "UnKnow Error"}
    };
    res.SetJsonType();
    res.BodyData(res_json.dump(2));
  }
}
/*
 *     "user": "0201302",
 */

void StudentMangeServer::app_token_parse(Request &req, Response &res, const NextFunc& next_func) {
  auto & body_json = req.json();
  if(body_json.find("token") == body_json.end())
    throw HttpException(403, "Unauthorized, not found token");
  Token token(body_json["token"].get<string>(), kTokenKey);
  if (!token.useful())
    throw HttpException(403, "Unauthorized, token error");
  else if(!token.in_time())
    throw HttpException(403, "Unauthorized, Login expired");
  body_json["user"] = token.get_user();
  next_func();
}

/**
 * @url /login
 * @input user text
 * @input password text
 */
void StudentMangeServer::app_login(const Request &req, Response &res, const NextFunc& next_func) {
  auto &req_body = req.json();
  TestJsonParam(req_body, {"user", "password"});
  Sqlite3::DBResType db_res;
  if (req_body["user"].get<string>().size() < 10)
    db_res = db_->CallCompiledSQL("teacher_password", req_body["user"].get<string>());
  else
    db_res = db_->CallCompiledSQL("student_password", req_body["user"].get<string>());

  if (db_res.size() != 1 || db_res[0][0].text_data != req_body["password"])
    throw HttpException(202, 400, "Accepted, password error or no that user.");

  Token token(req_body["user"], 24h);
  throw HttpException(json{{"token", token.Encode(kTokenKey)}});
}


/**
 * @url /self
 * @input token text
 */
void StudentMangeServer::app_self(const Request &req, Response &res, const NextFunc& next_func) {
  string user_id = req.json()["user"];
  Sqlite3::DBResType db_res;
  if (user_id.size() < 10) {
    db_res = db_->CallCompiledSQL("teacher_info", user_id);
    if (db_res.size() != 1) throw HttpException(404, "not found user");
    throw HttpException(json{{"data", {
        {"id", db_res[0][0].text_data},
        {"name", db_res[0][1].text_data},
        {"sex", db_res[0][2].text_data},
        {"age", db_res[0][3].int_data},
        {"tel", db_res[0][4].text_data},
    }}});
  }
  else {
    db_res = db_->CallCompiledSQL("student_info", user_id);
    if (db_res.size() != 1) throw HttpException(404, "not found user");
    throw HttpException(json{{"data", {
        {"id", db_res[0][0].text_data},
        {"name", db_res[0][1].text_data},
        {"specialty", db_res[0][2].text_data},
        {"class", db_res[0][3].text_data},
        {"sex", db_res[0][4].text_data},
        {"tel", db_res[0][5].text_data},
        {"instructor", db_res[0][7].text_data},
        {"mentor", db_res[0][8].text_data},
    }}});
  }
}

/**
 * @url /leave
 * @input token text "must student"
 * @input leave_type text
 * @input leave_reason text
 * @input time_begin text
 * @input time_end  text
 * @input is_school bool
 */
void StudentMangeServer::app_leave(const Request &req, Response &res, const NextFunc &next_func) {
  auto &req_body = req.json();
  string user_id = req_body["user"];
  TestJsonParam(req_body, {"leave_type", "leave_reason", "time_begin", "time_end", "is_school"});
  Sqlite3::DBResType db_res = db_->CallCompiledSQL("student_teachers", user_id);
  if (db_res.size() != 1) throw HttpException(404, "Not found student's teachers");
  string mentor_id = db_res[0][1].text_data;
  string instructor_id = db_res[0][0].text_data;
  string leave_type = req_body["leave_type"].get<string>();
  string leave_reason = req_body["leave_reason"].get<string>();
  string time_begin = req_body["time_begin"].get<string>();
  string time_end = req_body["time_end"].get<string>();
  bool is_school = req_body["is_school"].get<bool>();
  db_->CallCompiledSQL("insert_leave_info",
       user_id, mentor_id, instructor_id, leave_type, leave_reason, time_begin, time_end, is_school, PendingApproval);
}

/**
 * @url /release
 * @input token text "must student"
 * @input note_id number
*/
void StudentMangeServer::app_release(const Request &req, Response &res, const StudentMangeServer::NextFunc &next_func) {
  auto &req_body = req.json();
  string user_id = req_body["user"];
  TestJsonParam(req_body, {"note_id"});
  int note_id = static_cast<int>(req_body["note_id"].get<double>());
  Sqlite3::DBResType db_res = db_->CallCompiledSQL("select_leave_status", note_id);
  if (db_res.size() != 1) throw HttpException(404, "Not found leave info");
  auto status = static_cast<LeaveStatus>(db_res[0][0].int_data);
  if (status != LeaveStatus::Passed) throw HttpException(200, 403, "status must be passed.");
  db_->CallCompiledSQL("update_leave_info_status", note_id, Destroyed);
}