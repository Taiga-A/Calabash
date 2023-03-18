#include <iostream>
#include "web/server.h"
#include "sqlite3.h"

using namespace std;
using namespace calabash;

void hello(const Request &req, Response &resp) {
  string name = req.header("User-Agent");
  string age = req.path();
  string host = req.header("Host");
  resp.SetType(".html");
  resp.BodyData("hello, " + name + "," + age + "," + host);
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  int i;
  cout << "?" << endl;
  for (i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

int main() {
  sqlite3 *db;
  char *errmsg;
  char *data;
  int rc;

  rc = sqlite3_open("test.db", &db);
  sqlite3_exec(db, ".schema sqlite_master", callback, data, &errmsg);

  cout << "dat" << data;

  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    exit(0);
  } else {
    cout << "Opened database successfully\n";
  }
  sqlite3_close(db);

  auto *server = new Server();
  server->listen("", 8080);
  server->bind("/hello", hello);
  server->StaticPath("./static");
  server->start();
  return 0;
}
