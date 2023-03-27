#include <iostream>

#include "app/student_mange_server.h"

using namespace std;
using namespace calabash;

int main() {
  auto server = StudentMangeServer::Instance();

  server->Init(7777);
  server->Start();

  while(true) {
    string cmd;
    cin >> cmd;
    if (cmd == "exit")
      break;
    if (server->Status() && cmd == "stop")
      server->Stop();
    if (cmd == "status")
      cout << boolalpha << "run status: " << server->Status() << endl;
    if (!server->Status() && cmd == "start")
      server->Start();
  }
  cout << "wait..." << endl;
}
