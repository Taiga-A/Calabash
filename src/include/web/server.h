#pragma once

#include <string>
#include <functional>

#include "thread/thread_pool.h"
#include <socket/socket_handler.h>
#include <web/request.h>
#include <web/response.h>

namespace calabash {

typedef std::function<void(const Request &, Response &)> server_handler;

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

  void listen(const std::string &ip, int port);
  void start();
  void Stop();

  void set_threads(int threads);
  void set_connects(int connects);
  void set_wait_time(int wait_time);

  void bind(const std::string &path, server_handler handler);
  Response handle(const Request &req);
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
  std::map<std::string, server_handler> m_handlers;
};

}
