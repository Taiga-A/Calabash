//
// Created by Taiga on 2023/3/23.
//

#include "app/student_mange_server.h"
#include "util/logger.h"

using namespace calabash;
using namespace std;

void StudentMangeServer::Start() {
  ASSERT(is_init_)
  if (is_start_) {
    WARRING("http server have been started!");
    return;
  }
  this->server_thread_ = make_shared<thread>([&](){
    http_server_->Start();
  });
  is_start_ = true;
  SYSTEM("http server start!");
}

void StudentMangeServer::Stop() {
  if (!is_start_) {
    WARRING("http server have been stop!");
    return;
  }
  http_server_->Stop();
  server_thread_->join();
  server_thread_ = nullptr;
  is_start_ = false;
}

void StudentMangeServer::Init(int port, const std::string &db_name) {
  db_ = make_shared<Sqlite3>(db_name);
  DataBaseInit();
  http_server_ = make_shared<Server>();
  http_server_->Listen("", port);
  // server init  [👇]  ↓


  is_init_ = true;
}

void StudentMangeServer::DataBaseInit() {
  auto create_db = ""
                   "";
//  db_->SQL();
}
