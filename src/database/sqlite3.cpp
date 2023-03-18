//
// Created by Taiga on 2023/3/18.
//

#include "database/sqlite3.h"

using namespace std;
using namespace calabash;

[[maybe_unused]] Sqlite3::Sqlite3(const std::string &database_path) {
  if (sqlite3_open(database_path.c_str(), &db_) != SQLITE_OK) {
    ERROR(string("Can't open database: ") + sqlite3_errmsg(db_), -1);
  } else {
    SYSTEM("Open database successful!");
    CONSOLE("Open database successful!");
  }
}

Sqlite3::~Sqlite3() {
  for (auto &[th, stmt] : sql_map_) {
    int del_status;
    if ((del_status = sqlite3_finalize(stmt)) != SQLITE_OK) {
      ERROR(string("Sql del err :") + th + " " + sqlite3_errmsg(db_) + " " + to_string(del_status));
    }
  }
  if (sqlite3_close(db_) != SQLITE_OK) {
    ERROR(string("Can't close database: ") + sqlite3_errmsg(db_), -1);
  }
}

vector<vector<Sqlite3Data>> Sqlite3::SQL(const string &sql) {
  sqlite3_stmt *sql_program = CompileSQL(sql);
  if (sql_program == nullptr) return {};
  auto ret = GetDataFromStmt(sql_program);
  int del_status;
  if ((del_status = sqlite3_finalize(sql_program)) != SQLITE_OK) {
    ERROR(string("Sql del err :") + sqlite3_errmsg(db_) + " " + to_string(del_status));
  } else {
    INFO("Run SQL Successful! " + sql);
  }
  return ret;
}

void Sqlite3::BindCompiledSQL(const string &sql_name, const string &sql) {
  sqlite3_stmt *sql_res = nullptr;
  int status = sqlite3_prepare_v2(
      db_,
      sql.c_str(),
      static_cast<int>(sql.size()),
      &sql_res,
      nullptr);
  if (status != SQLITE_OK) {
    ERROR("SQL compilation failed: " + sql_name + " " + sqlite3_errmsg(db_), -1);
  } else {
    INFO("Bind SQL_Compiled Successful! " + sql_name);
  }
  sql_map_.insert({sql_name, sql_res});
}

sqlite3_stmt *Sqlite3::CompileSQL(const string &sql) {
  sqlite3_stmt *sql_res = nullptr;
  int status = sqlite3_prepare_v2(
      db_,
      sql.c_str(),
      static_cast<int>(sql.size()),
      &sql_res,
      nullptr);
  if (status != SQLITE_OK) {
    ERROR("SQL compilation failed: \n\t\t" + sql + "\n\t\t" + sqlite3_errmsg(db_), -1);
    return nullptr;
  } else {
    INFO("Compiled SQL Successful! " + sql);
  }
  return sql_res;
}

std::vector<std::vector<Sqlite3Data>> Sqlite3::GetDataFromStmt(sqlite3_stmt *stmt) {
  DBResType ret;
  int run_status;
  while ((run_status = sqlite3_step(stmt)) == SQLITE_ROW) {
    vector<Sqlite3Data> column;
    int column_num = sqlite3_column_count(stmt);
    for (int i = 0; i < column_num; i++) {
      column.push_back(FmtData(stmt, i));
    }
    ret.emplace_back(std::move(column));
  }
  if (run_status != SQLITE_DONE) {
    ERROR(string("Sql data err :") + sqlite3_errmsg(db_) + " " + to_string(run_status));
    return {};
  }
  return ret;
}

Sqlite3Data Sqlite3::FmtData(sqlite3_stmt *stmt, int index) {
  Sqlite3Data dat{};
  dat.type = static_cast<DBValType>(sqlite3_column_type(stmt, index));
  switch (dat.type) {
    case DBValType::BLOB :
      dat.text_data = string(
          reinterpret_cast<const char *>(sqlite3_column_blob(stmt, index)),
          sqlite3_column_bytes(stmt, index));
      break;
    case DBValType::INTEGER:dat.int_data = sqlite3_column_int(stmt, index);
      break;
    case DBValType::DOUBLE:dat.double_data = sqlite3_column_double(stmt, index);
      break;
    case DBValType::TEXT:
      dat.text_data = string(
          reinterpret_cast<const char *>(sqlite3_column_blob(stmt, index)),
          sqlite3_column_bytes(stmt, index));
      break;
    case DBValType::DAT_NULL:
    default:break;
  }
  return dat;
}

std::string Sqlite3Data::to_string() const {
  switch (type) {
    case Sqlite3Data::DataType::INTEGER :return std::to_string(int_data);
    case Sqlite3Data::DataType::DOUBLE :return std::to_string(double_data);
    case Sqlite3Data::DataType::TEXT :
    case Sqlite3Data::DataType::BLOB :return text_data;
    default:return "null";
  }
}
