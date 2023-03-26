//
// Created by Taiga on 2023/3/26.
//

#include <utility>

#include "app/exception.h"

using namespace std;
using namespace nlohmann;

HttpException::HttpException(int base_code) :
    HttpException(base_code, "ok") {
}

HttpException::HttpException(const json &body) :
    HttpException(200, 200, "ok", body) {

}

HttpException::HttpException(int base_code, const string &msg) :
    HttpException(base_code, base_code, msg) {

}

HttpException::HttpException(int base_code, int code, const string &msg) :
    HttpException(base_code, code, msg, nlohmann::json::object()) {

}

HttpException::HttpException(int base_code, int code, string msg, json body) :
    base_code_(base_code),
    code_(code),
    body_(std::move(body)),
    msg_(std::move(msg)) {
}



