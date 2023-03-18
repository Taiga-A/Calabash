#include <socket/server_socket.h>

#include <util/logger.h>

#include <cerrno>
#include <cstring>

using namespace std;
using namespace calabash;

ServerSocket::ServerSocket() : Socket() {}

ServerSocket::ServerSocket(const string &ip, int port) : Socket(ip, port) {
  m_sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (m_sockfd < 0) {
    ERROR("create server socket error " + string(strerror(errno)));
    return;
  }

  set_non_blocking();
  set_recv_buffer(10 * 1024);
  set_send_buffer(10 * 1024);
  set_linger(true, 0);
  set_keep_alive();
  set_reuse_addr();
  bind(ip, port);
  listen(1024);
}

ServerSocket::~ServerSocket() {
  close();
}
