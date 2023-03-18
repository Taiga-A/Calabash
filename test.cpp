//
// Created by Taiga on 2023/3/8.
//

#include "database/sqlite3.h"
#include "util/logger.h"

using namespace std;
using namespace calabash;

int main() {
  Sqlite3 db("./test.db");
  db.BindCompiledSQL("insert", "INSERT INTO students (name, id, class, age) VALUES (?1, ?2, ?3, ?4)");
  db.BindCompiledSQL("update_id_from_name", "UPDATE students SET id=?2 WHERE name=?1");
  db.BindCompiledSQL("update_age_from_name", "UPDATE students SET age=?2 WHERE name=?1");
  db.BindCompiledSQL("show_all", "SELECT * FROM students");

  auto res = db.CallCompiledSQL("show_all");
  print(res);

  return 0;
}