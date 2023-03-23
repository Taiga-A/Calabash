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

int main() {
  Server server;
  server.Listen("", 8080);
  server.Bind("/hello", hello);
  server.StaticPath("./static");


  thread t = thread([&](){
    cout << "start1" << endl;
    server.Start();
  });

  this_thread::sleep_for(10s);
  cout << "stop" << endl;
  server.Stop();
  t.join();



  t = thread([&](){
    cout << "start2" << endl;
    server.Start();
  });
  this_thread::sleep_for(10s);
  cout << "stop" << endl;
  server.Stop();
  t.join();
}
