#pragma once

#include "socket/socket.h"
#include "socket/socket_handler.h"

namespace calabash {

class Server;

const uint32_t recv_buff_size = 1024 * 1024 * 4;

class HttpTask {
 public:
  void operator()(Server *server, SocketHandler *, Socket *socket);
};

}
