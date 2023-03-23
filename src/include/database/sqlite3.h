//
// Created by Taiga on 2023/3/18.
//

#ifndef CALABASH_INCLUDE_DATABASE_SQLITE3_H_
#define CALABASH_INCLUDE_DATABASE_SQLITE3_H_

#include <string>
#include <vector>
#include <map>

#include <sqlite3.h>

#include "util/logger.h"

namespace calabash {

struct Sqlite3Data {
  enum DataType {
    INTEGER = SQLITE_INTEGER,
    DOUBLE = SQLITE_FLOAT,
    BLOB = SQLITE_BLOB,
    DAT_NULL = SQLITE_NULL,
    TEXT = SQLITE3_TEXT,
  };
  std::string text_data;
  double double_data;
  int int_data;
  DataType type;
  [[nodiscard]] std::string to_string() const;
};

class Sqlite3 {
 public:
  using DBResType = std::vector<std::vector<Sqlite3Data>>;
  using DBValType = Sqlite3Data::DataType;

  [[maybe_unused]] explicit Sqlite3(const std::string &database_path);
  ~Sqlite3();

  [[maybe_unused]] DBResType SQL(const std::string &sql);
  [[maybe_unused]] void BindCompiledSQL(const std::string &sql_name, const std::string &sql);

  template<class ...Arg>
  [[maybe_unused]] DBResType CallCompiledSQL(const std::string &sql_name, Arg...);

 private:
  [[maybe_unused]] static inline void BindCompiledSQLParam(sqlite3_stmt *, int index);
  template<class ...Arg>
  [[maybe_unused]] void BindCompiledSQLParam(sqlite3_stmt *, int, int, Arg...);
  template<class ...Arg>
  [[maybe_unused]] void BindCompiledSQLParam(sqlite3_stmt *, int, double, Arg...);
  template<class ...Arg>
  [[maybe_unused]] void BindCompiledSQLParam(sqlite3_stmt *, int, const std::string &, Arg...);
  template<class ...Arg>
  [[maybe_unused]] void BindCompiledSQLParam(sqlite3_stmt *, int, void *, Arg...);

  sqlite3_stmt *CompileSQL(const std::string &sql);
  DBResType GetDataFromStmt(sqlite3_stmt *stmt);
  static Sqlite3Data FmtData(sqlite3_stmt *stmt, int index);

 private:
  sqlite3 *db_{};
  std::multimap<std::string, sqlite3_stmt *> sql_map_;
};

inline std::ostream &operator<<(std::ostream &os, const Sqlite3Data &dat) {
  return os << dat.to_string();
}

template<class... Arg>
std::vector<std::vector<Sqlite3Data>> Sqlite3::CallCompiledSQL(const std::string &sql_name, Arg... arg) {
  if (sql_map_.find(sql_name) == sql_map_.end()) {
    WARRING("Not found such SQL name like: " + sql_name);
    return {};
  }
  sqlite3_stmt *stmt = sql_map_.find(sql_name)->second;
  BindCompiledSQLParam(stmt, 1, arg...);
  auto res = GetDataFromStmt(stmt);
  int reset_status = sqlite3_reset(stmt);
  if (reset_status != SQLITE_OK) {
    ERROR(std::string("Sql reset err :") + sqlite3_errmsg(db_) + " " + std::to_string(reset_status));
  } else {
    INFO("Call SQL " + sql_name + " Successful! Result rows " + std::to_string(res.size()));
  }
  return res;
}

void Sqlite3::BindCompiledSQLParam(sqlite3_stmt *, int index) {
//  INFO("Bind SQL Params Successful! Param num : " + std::to_string(index - 1));
}

template<class... Arg>
[[maybe_unused]] void Sqlite3::BindCompiledSQLParam(sqlite3_stmt *stmt, int index, int val, Arg... arg) {
  int bind_status = sqlite3_bind_int(stmt, index, val);
  if (bind_status != SQLITE_OK) {
    ERROR(std::string("Sql bind err :") + sqlite3_errmsg(db_) + " " + std::to_string(bind_status));
  }
  BindCompiledSQLParam(stmt, index + 1, arg...);
}

template<class... Arg>
[[maybe_unused]] void Sqlite3::BindCompiledSQLParam(sqlite3_stmt *stmt, int index, double val, Arg... arg) {
  int bind_status = sqlite3_bind_double(stmt, index, val);
  if (bind_status != SQLITE_OK) {
    ERROR(std::string("Sql bind err :") + sqlite3_errmsg(db_) + " " + std::to_string(bind_status));
  }
  BindCompiledSQLParam(stmt, index + 1, arg...);
}

template<class... Arg>
[[maybe_unused]] void Sqlite3::BindCompiledSQLParam(sqlite3_stmt *stmt, int index, const std::string &val, Arg... arg) {
  int bind_status = sqlite3_bind_text(stmt, index, val.c_str(), static_cast<int>(val.size()), SQLITE_TRANSIENT);
  if (bind_status != SQLITE_OK) {
    ERROR(std::string("Sql bind err :") + sqlite3_errmsg(db_) + " " + std::to_string(bind_status));
  }
  BindCompiledSQLParam(stmt, index + 1, arg...);
}

template<class... Arg>
[[maybe_unused]] void Sqlite3::BindCompiledSQLParam(sqlite3_stmt *stmt, int index, void *n_ptr, Arg... arg) {
  if (n_ptr == nullptr) {
    int bind_status = sqlite3_bind_null(stmt, index);
    if (bind_status != SQLITE_OK) {
      ERROR(std::string("Sql bind err :") + sqlite3_errmsg(db_) + " " + std::to_string(bind_status));
    }
  }
  BindCompiledSQLParam(stmt, index + 1, arg...);
}

} // namespace calabash

#endif //CALABASH_INCLUDE_DATABASE_SQLITE3_H_

