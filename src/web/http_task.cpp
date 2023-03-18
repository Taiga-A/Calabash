
#include "web/http_task.h"
#include "util/logger.h"
#include "web/request_maker.h"
#include "web/server.h"

using namespace calabash;
using namespace std;

void HttpTask::operator()(Server *server, SocketHandler *handler, Socket *socket) {
  Request req;
  if (RequestMaker::Parse(socket, &req)) {
    Response res = server->handle(req);
    DataReader &data_reader = res.GetDataReader();

    data_reader.BufferRead([&](const char *buf, size_t size) -> size_t {
      return socket->send(buf, size);
    });

    SYSTEM(socket->ip() + " " + req.method() + " " + to_string(res.code()) +
        "\t" + to_string(req.body_size()) + "Byte " + "--> " +
        to_string(static_cast<double>(data_reader.Size()) / 1024 / 1024) + "MB\t"
               + req.path());
    handler->attach(socket);
  } else {
    handler->remove(socket);
  }
}
