#include <socket/socket_handler.h>
#include <socket/server_socket.h>

#include <utility>

#include "util/logger.h"

using namespace std;
using namespace calabash;

SocketHandler::SocketHandler(std::function<void(SocketHandler *, Socket *)> &&func, shared_ptr<ThreadPool> thread_pool)
    : object_pool_(ObjectPool<Socket>::Instance()), thread_pool_(std::move(thread_pool)), func_(std::move(func)) {}

SocketHandler::~SocketHandler() {
  if (m_epoll != nullptr) {
    delete m_epoll;
    m_epoll = nullptr;
  }
  if (m_server != nullptr) {
    delete m_server;
    m_server = nullptr;
  }
}

void SocketHandler::listen(const string &ip, int port) {
  m_server = new ServerSocket(ip, port);
}

void SocketHandler::attach(Socket *socket) {
  auto mutex = lock_guard(mutex_);
  m_epoll->add(socket->m_sockfd, (void *) socket, (EPOLLONESHOT | EPOLLIN | EPOLLHUP | EPOLLERR));
}

void SocketHandler::detach(Socket *socket) {
  auto mutex = lock_guard(mutex_);
  m_epoll->del(socket->m_sockfd, (void *) socket, (EPOLLONESHOT | EPOLLIN | EPOLLHUP | EPOLLERR));
}

void SocketHandler::remove(Socket *socket) {
  socket->close();
  object_pool_->Delete(socket);
}

void SocketHandler::handle(int max_connections, int wait_time) {
  is_open_ = true;
  m_epoll = new EventPoller(false);
  m_epoll->create(max_connections);
  m_epoll->add(m_server->m_sockfd, m_server, (EPOLLIN | EPOLLHUP | EPOLLERR));
  object_pool_->SetMaxSize(max_connections);

  while (is_open_) {
    int num = m_epoll->wait(wait_time);
    if (num == 0) {
      continue;
    }
    for (int i = 0; i < num; i++) {
      if (m_server == static_cast<Socket *>(m_epoll->m_events[i].data.ptr)) {
        struct sockaddr_in addr{};
        socklen_t addr_size = sizeof(struct sockaddr_in);
        int soctfd = m_server->accept(&addr, &addr_size);
        Socket *socket = object_pool_->Allocate();
        if (socket == nullptr) {
          ERROR("socket pool is empty");
          break;
        }
        socket->m_sockfd = soctfd;
        socket->m_ip = inet_ntoa(addr.sin_addr);
        socket->set_non_blocking();
        attach(socket);
      } else {
        auto socket = static_cast<Socket *>(m_epoll->m_events[i].data.ptr);
        if (m_epoll->m_events[i].events & EPOLLHUP) {
          ERROR("socket closed by peer id :" + to_string(socket->m_sockfd));
          detach(socket);
          remove(socket);
        } else if (m_epoll->m_events[i].events & EPOLLERR) {
          ERROR("socket error id :" + to_string(socket->m_sockfd));
          detach(socket);
          remove(socket);
        } else if (m_epoll->m_events[i].events & EPOLLIN) {
          detach(socket);
          thread_pool_->Enqueue(func_, this, socket);
        }
      }
    }
  }
}

void SocketHandler::stop() {
  is_open_ = false;
  if (m_epoll != nullptr) {
    delete m_epoll;
    m_epoll = nullptr;
  }
}
