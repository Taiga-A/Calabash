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

namespace calabash {

#define HTTP_ERROR(res, base_code, msg, code) {        \
  res.set_code(base_code);                             \
  json body;                                           \
  body["code"] = code;                                 \
  body["msg"] = msg;                                   \
  res.SetJsonType();                                   \
  res.BodyData(to_string(body));                       \
  return;                                              \
}

#define HTTP_METHOD_POST(req, res)        \
if (req.method() != "POST")               \
  HTTP_ERROR(res, 405, "Method Not Allowed, you must use POST method", 405)

#define JSON_TRY_PARSE(json_t, str, res)  \
try {                                     \
  json_t = json::parse(str);              \
} catch (const json::exception & e)       \
  HTTP_ERROR(res, 400, "Bad Request, server can't understand!", 400)


#define TOKEN_AUTH(json_t, user, res)                                                           \
if(!json_t["token"].is_string())                                                                \
  HTTP_ERROR(res, 403, "Unauthorized, not found token", 403)                                    \
Token token(json_t["token"].get<string>(), kTokenKey);                                          \
if (!token.useful())                                                                            \
  HTTP_ERROR(res, 403, "Unauthorized, token error", 403)                                        \
else if(!token.in_time())                                                                       \
  HTTP_ERROR(res, 403, "Unauthorized, Login expired", 403)                                      \
user = token.get_user();

} // namespace calabash


/**
 * @url /login
 * @input user 用户id
 * @input password 密码
 */
void StudentMangeServer::app_login(const Request &req, Response &res) {
  json req_body;
  HTTP_METHOD_POST(req, res)
  JSON_TRY_PARSE(req_body, req.body(), res)
  if (!req_body["user"].is_string() || !req_body["password"].is_string())
    HTTP_ERROR(res, 403, "Forbidden, server can understand, but refusal of execution. "
                       "The general reason is the lack of necessary parameters", 403)

  Sqlite3::DBResType db_res;
  if (req_body["user"].get<string>().size() < 10)
    db_res = db_->CallCompiledSQL("teacher_password", req_body["user"].get<string>());
  else
    db_res = db_->CallCompiledSQL("student_password", req_body["user"].get<string>());

  if (db_res.size() != 1 || db_res[0][0].text_data != req_body["password"]) {
    res.set_code(202);
    json body;
    body["code"] = 400;
    body["msg"] = "Accepted, password error or no that user.";
    res.SetJsonType();
    res.BodyData(to_string(body));
    return;
  }

  Token token(req_body["user"], 30s);
  json body;
  body["token"] = token.Encode(kTokenKey);
  body["code"] = 200;
  body["msg"] = "ok";
  res.set_code(200);
  res.SetJsonType();
  res.BodyData(to_string(body));
}


/**
 * @url /self
 * @input token
 */
void StudentMangeServer::app_self(const Request &req, Response &res) {
  json req_body, res_body;
  string user_id;
  HTTP_METHOD_POST(req, res)
  JSON_TRY_PARSE(req_body, req.body(), res)
  TOKEN_AUTH(req_body, user_id, res)

  Sqlite3::DBResType db_res;
  if (user_id.size() < 10) {
    db_res = db_->CallCompiledSQL("teacher_info", user_id);
    if (db_res.size() != 1) HTTP_ERROR(res, 404, "not found user", 404)
    res_body["id"] = db_res[0][0].text_data;
    res_body["name"] = db_res[0][1].text_data;
    res_body["sex"] = db_res[0][2].text_data;
    res_body["age"] = db_res[0][3].int_data;
    res_body["tel"] = db_res[0][4].text_data;
  }
  else {
    db_res = db_->CallCompiledSQL("student_info", user_id);
    if (db_res.size() != 1) HTTP_ERROR(res, 404, "not found user", 404)
    res_body["id"] = db_res[0][0].text_data;
    res_body["name"] = db_res[0][1].text_data;
    res_body["specialty"] = db_res[0][2].text_data;
    res_body["class"] = db_res[0][3].text_data;
    res_body["sex"] = db_res[0][4].text_data;
    res_body["tel"] = db_res[0][5].text_data;
    res_body["instructor"] = db_res[0][7].text_data;
    res_body["mentor"] = db_res[0][8].text_data;
  }
  res.set_code(200);
  res.SetJsonType();
  res_body["msg"] = "ok";
  res.BodyData(to_string(res_body));
}

