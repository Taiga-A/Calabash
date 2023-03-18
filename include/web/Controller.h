#pragma once

#include <web/request.h>
#include <web/response.h>

#include <reflect/ClassRegister.h>
using namespace yazi::reflect;

namespace yazi {
namespace web {

class Controller : public Object {
 public:
  Controller() {}
  virtual ~Controller() {}
};

}
}