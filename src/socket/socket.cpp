#include <socket/socket.h>

#include "util/logger.h"

#include <cstring>
#include <utility>

using namespace std;
using namespace calabash;

Socket::Socket() : m_sockfd(0) {}

Socket::Socket(string ip, int port) : m_ip(std::move(ip)), m_port(port), m_sockfd(0) {}

Socket::~Socket() {
  close();
}

bool Socket::bind(const string &ip, int port) const {
  struct sockaddr_in sockaddr{};
  memset(&sockaddr, 0, sizeof(sockaddr));
  sockaddr.sin_family = AF_INET;
  if (ip != "") {
    sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
  } else {
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  }
  sockaddr.sin_port = htons(port);

  if (::bind(m_sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
    ERROR("bind");
    return false;
  }
  return true;
}

bool Socket::listen(int backlog) const {
  if (::listen(m_sockfd, backlog) < 0) {
    ERROR("listen");
    return false;
  }
  return true;
}

bool Socket::connect(const string &ip, int port) const {
  struct sockaddr_in sockaddr;
  memset(&sockaddr, 0, sizeof(sockaddr));
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
  sockaddr.sin_port = htons(port);
  if (::connect(m_sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
    ERROR("connect");
    return false;
  }
  return true;
}

bool Socket::close() {
  if (m_sockfd > 0) {
    ::close(m_sockfd);
    m_sockfd = 0;
  }
  return true;
}

int Socket::accept(sockaddr_in *addr, socklen_t *addr_size) const {
  int sockfd = ::accept(m_sockfd, reinterpret_cast<sockaddr *>(addr), addr_size);
  if (sockfd < 0) {
    ERROR("accept");
    sockfd = -1;
  }
  return sockfd;
}

ssize_t Socket::recv(char *buf, size_t len) const {
  return ::recv(m_sockfd, buf, len, 0);
}

size_t Socket::send(const char *buf, size_t len) const {
  size_t send_size;
  while ((send_size = ::send(m_sockfd, buf, len, 0)) == -1);
  return send_size;
}

bool Socket::set_non_blocking() const {
  int flags = fcntl(m_sockfd, F_GETFL, 0);
  if (flags < 0) {
    ERROR("non_blocking");
    return false;
  }
  flags |= O_NONBLOCK;
  if (fcntl(m_sockfd, F_SETFL, flags) < 0) {
    ERROR("non_blocking");
    return false;
  }
  return true;
}

bool Socket::set_send_buffer(int size) const {
  int buff_size = size;
  if (setsockopt(m_sockfd, SOL_SOCKET, SO_SNDBUF, &buff_size, sizeof(buff_size)) < 0) {
    ERROR("send_buf");
    return false;
  }
  return true;
}

bool Socket::set_recv_buffer(int size) const {
  int buff_size = size;
  if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, &buff_size, sizeof(buff_size)) < 0) {
    ERROR("recv_buf");
    return false;
  }
  return true;
}

bool Socket::set_linger(bool active, int seconds) const {
  struct linger l{};
  memset(&l, 0, sizeof(l));

  if (active)
    l.l_onoff = 1;
  else
    l.l_onoff = 0;
  l.l_linger = seconds;

  if (setsockopt(m_sockfd, SOL_SOCKET, SO_LINGER, &l, sizeof(l)) < 0) {
    ERROR("linger");
    return false;
  }
  return true;
}

bool Socket::set_keep_alive() const {
  int flag = 1;
  if (setsockopt(m_sockfd, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag)) < 0) {
    ERROR("keep_alive");
    return false;
  }
  return true;
}

bool Socket::set_reuse_addr() const {
  int flag = 1;
  if (setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0) {
    ERROR("reuse_addr");
    return false;
  }
  return true;
}

bool Socket::set_reuse_port() const {
  int flag = 1;
  if (setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(flag)) < 0) {
    ERROR("reuse_port");
    return false;
  }
  return true;
}
