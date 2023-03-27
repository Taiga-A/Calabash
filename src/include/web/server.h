#pragma once

#include <string>
#include <functional>

#include "thread/thread_pool.h"
#include <socket/socket_handler.h>
#include <web/request.h>
#include <web/response.h>

namespace calabash {

using ServerNextFunc = std::function<void(void)>;
using ServerFunc = std::function<void(Request&, Response&, ServerNextFunc)>;

struct ServerConfig {
  std::string ip = "0.0.0.0";
  int port = 8080;
  int threads = 30;
  int connects = 100;
  int wait_time = 5000;
};

class Server {
 public:
  explicit Server(const ServerConfig &server_config = {});
  ~Server() = default;

  void Listen(const std::string &ip, int port);
  void Start();
  void Stop();

  void set_threads(int threads);
  void set_connects(int connects);
  void set_wait_time(int wait_time);

  inline void Bind(const std::string &path, const ServerFunc &handler);
  inline void Bind(const std::string &path, std::initializer_list<ServerFunc> handler);
  inline void ProcessBind(const ServerFunc &handler);
  Response Handle(Request &req);
  void StaticPath(std::string path);

 private:
  std::shared_ptr<ThreadPool> thread_pool_;
  std::shared_ptr<SocketHandler> socket_handler_;
  std::string m_ip;
  int m_port;
  int m_threads;
  int m_connects;
  int m_wait_time;
  std::string m_root_path;
  std::string static_path_;
  std::map<std::string, std::vector<ServerFunc>> handlers_;
  std::vector<ServerFunc> process_handles;
};

void Server::Bind(const std::string &path, const ServerFunc &handler) {
  handlers_[path].emplace_back(handler);
}

void Server::Bind(const std::string &path, std::initializer_list<ServerFunc> handler) {
  for (auto & func_ref : handler) {
    handlers_[path].emplace_back(func_ref);
  }
}

void Server::ProcessBind(const ServerFunc &handler) {
  process_handles.emplace_back(handler);
}

}
