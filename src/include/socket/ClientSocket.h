#pragma once

#include <string>
using namespace std;

#include <socket/socket.h>

namespace yazi {
namespace socket {

class ClientSocket : public Socket {
 public:
  ClientSocket();
  ClientSocket(const string &ip, int port);
  virtual ~ClientSocket();
};

}}
