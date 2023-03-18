#pragma once

#include <string>
using namespace std;

#include <socket/socket.h>

namespace calabash {

class ServerSocket : public Socket {
 public:
  ServerSocket();
  ServerSocket(const string &ip, int port);
  virtual ~ServerSocket();
};

}
