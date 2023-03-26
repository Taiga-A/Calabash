#include <sstream>
#include <utility>

#include <util/logger.h>
#include "web/server.h"
#include "web/http_task.h"

using namespace calabash;
using namespace std;

Server::Server(const ServerConfig &server_config) {
  m_ip = server_config.ip;
  m_port = server_config.port;
  m_threads = server_config.threads;
  m_connects = server_config.connects;
  m_wait_time = server_config.wait_time;
}

void Server::Listen(const string &ip, int port) {
  m_ip = ip;
  m_port = port;
}

void Server::Start() {
  thread_pool_ = make_shared<ThreadPool>(m_threads);
  auto func = [this](auto &&PH1, auto &&PH2) {
    return HttpTask()(this,
                      std::forward<decltype(PH1)>(PH1),
                      std::forward<decltype(PH2)>(PH2));
  };
  socket_handler_ = make_shared<SocketHandler>(func, thread_pool_);
  SYSTEM("Listen at port " + to_string(m_port));
  socket_handler_->listen(m_ip, m_port);
  socket_handler_->handle(m_connects, m_wait_time);
}

void Server::set_threads(int threads) {
  m_threads = threads;
}

void Server::set_connects(int connects) {
  m_connects = connects;
}

void Server::set_wait_time(int wait_time) {
  m_wait_time = wait_time;
}

void Server::StaticPath(string path) {
  static_path_ = std::move(path);
}

Response Server::Handle(Request &req) {
  const string &path = req.path();

  auto it = handlers_.find(path);
  Response resp;
  if (it != handlers_.end()) {
    auto &func_list = it->second;
    int process_func_index = -1;
    int func_index = -1;
    function<void()> next_func = [&]() {
      if (process_func_index + 1 < process_handles.size()) {
        process_func_index++;
        process_handles[process_func_index](req, resp, next_func);
      } else if (func_index + 1 < func_list.size()) {
        func_index++;
        func_list[func_index](req, resp, next_func);
      } else if (func_index + 1 >= func_list.size()) {
        WARNING("Func_list is over, call with nothing at " + path + " " + to_string(func_index));
      }
    };

    next_func();

    return resp;
  }

  // static file path
  if (!static_path_.empty()) {
    filesystem::path now_path = static_path_ + req.path();
    if (!filesystem::is_regular_file(now_path))
      now_path = static_path_ + "/index.html";
    if (filesystem::is_regular_file(now_path)) {
      resp.AutoFileData(now_path);
      resp.AutoSetType(now_path);
      return resp;
    }
  }

  resp.page_not_found();
  return resp;
}

void Server::Stop() {
  socket_handler_->stop();
  socket_handler_ = nullptr;
  thread_pool_ = nullptr;
}


