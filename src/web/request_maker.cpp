//
// Created by Taiga on 2023/3/16.
//

#include <algorithm>
#include <fstream>

#include "web/request_maker.h"

using namespace calabash;
using namespace std;

string RequestMaker::kNumMap = "0123456789ABCDEF";
string RequestMaker::kAsciiMap = R"("%<>[\]^_`{|})";

bool RequestMaker::Parse(Socket *socket, Request *req) {
  char buf[kReqBufSize];
  ssize_t base_size = socket->recv(buf, kReqBufSize);
  if (base_size <= 0) return false;
  size_t head_size;
  if ((head_size = ParseHttpHeader(buf, req)) == 0) return false;
  req->body_size_ = ::strtol(req->header("Content-Length").c_str(), nullptr, 10);
  req->from_ip_ = socket->ip();

  if (TestContentTypeMultipart(req->header("Content-Type"))) {
    SaveMultipartFile(buf, buf + head_size, base_size - head_size, req);
  } else {
    req->body_ = string(buf + head_size, base_size - head_size);
    size_t real_read = base_size - head_size;
    while (real_read < req->body_size_) {
      ssize_t th_read = socket->recv(buf, kReqBufSize);
      if (th_read <= 0) return false;
      req->body_ += string(buf, th_read);
      real_read += th_read;
    }
  }

  if (TestContentTypeJson(req->header("Content-Type"))) {
    req->is_json_ = true;
  }

  return true;
}

size_t RequestMaker::ParseHttpHeader(const char *buf, Request *req) {
  const char *start_point = buf;
  buf = GetNextWord(buf, &req->method_);
  buf = GetNextWord(buf, &req->path_);
  buf = GetNextWord(buf, &req->version_);

  string headers;
  buf = GetNextLine(buf, &headers);
  while (!headers.empty()) {
    string key, val;
    headers.push_back('\n');
    GetNextLine(GetNextWord(headers.c_str(), &key), &val);
    key.pop_back();
    req->headers_.emplace(std::move(key), std::move(val));
    buf = GetNextLine(buf, &headers);
  }

  auto path_strings = Split(req->path_, '?');
  req->path_ = UrlDecode(path_strings[0]);
  if (path_strings.size() == 2) {
    path_strings = Split(path_strings[1], '&');
    for (auto &th_parm : path_strings) {
      int index = 0;
      while (th_parm[index] != '=') index++;
      string param_key(th_parm.c_str(), index++);
      param_key = UrlDecode(param_key);
      while (index < th_parm.size()) index++;
      string param_val(th_parm.c_str() + param_key.size(), index - param_key.size());
      param_val = UrlDecode(param_val);
      req->params_.emplace(std::move(param_key), std::move(param_val));
    }
  }

  return (buf - start_point);
}

void RequestMaker::SaveMultipartFile(const char *buf, const char *start, size_t len, Request *req) {

}

string RequestMaker::UrlEncode(const string &url) {
  string ret;
  for (char it : url) {
    if (((it >> 7) & 1) || (count(begin(kAsciiMap), end(kAsciiMap), it))) {
      ret.push_back('%');
      ret.push_back(kNumMap[(it >> 4) & 0x0F]);
      ret.push_back(kNumMap[it & 0x0F]);
    } else {
      ret.push_back(it);
    }
  }
  return ret;
}

string RequestMaker::UrlDecode(const string &url) {
  string ret;
  for (auto it = url.begin(); it != url.end(); ++it) {
    if (*it == '%') {
      if (next(it++) == url.end()) {
        throw invalid_argument("url is invalid");
      }
      ret.push_back(RequestMaker::UrlCodeGetHEX(*it, *next(it)));
      if (next(it++) == url.end()) {
        throw invalid_argument("url is invalid");
      }
    } else {
      ret.push_back(*it);
    }
  }
  return ret;
}
