#include <iostream>
#include "web/server.h"
#include "app/student_mange_server.h"
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

int main() {
  auto server = StudentMangeServer::Instance();

  server->Init(7777);
  server->Start();

  while(true);
}
