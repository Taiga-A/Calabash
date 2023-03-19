#pragma once

#include <list>
#include <string>
#include "functional"

#include "socket/socket.h"
#include "socket/event_poller.h"
#include "util/object_pool.h"
#include "thread/thread_pool.h"

namespace calabash {

class SocketHandler {
 public:
  SocketHandler(std::function<void(SocketHandler *, Socket *)> &&func, std::shared_ptr<ThreadPool> thread_pool);
  ~SocketHandler();

  void listen(const std::string &ip, int port);
  void attach(Socket *socket);
  void detach(Socket *socket);
  void remove(Socket *socket);
  void handle(int max_connections, int wait_time);
  void stop();

 private:
  bool is_open_ = true;
  EventPoller *m_epoll{};
  Socket *m_server{};
  std::mutex mutex_;
  ObjectPool<Socket> *object_pool_;
  std::shared_ptr<ThreadPool> thread_pool_;
  std::function<void(SocketHandler *, Socket *)> func_;
};

}
