#pragma once

#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include <string>

namespace calabash {

class Socket {
  friend class SocketHandler;

 public:
  Socket();
  Socket(std::string ip, int port);
  virtual ~Socket();

  bool bind(const std::string &ip, int port) const;
  bool listen(int backlog) const;
  bool connect(const std::string &ip, int port) const;
  bool close();

  int accept(sockaddr_in *addr, socklen_t *addr_size) const;
  ssize_t recv(char *buf, size_t len) const;
  size_t send(const char *buf, size_t len) const;

  bool set_non_blocking() const;
  bool set_send_buffer(int size) const;
  bool set_recv_buffer(int size) const;
  bool set_linger(bool active, int seconds) const;
  bool set_keep_alive() const;
  bool set_reuse_addr() const;
  bool set_reuse_port() const;

  inline std::string ip() const { return m_ip; }

 protected:
  std::string m_ip;
  int m_port{};
  int m_sockfd;
};

}
