#include <web/response.h>

#include <sstream>
#include <map>

using namespace std;
using namespace calabash;

map<int, string> kHttpCodeWithInfo = {
#include "inc/HttpCodeWithInfo.inc"
};

map<string, string> kHttpExtensionWithContentType = {
#include "inc/HttpContentTypeWithExtension.inc"
};

void Response::page_not_found() {
  string body = "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta charset=\"utf-8\">"
                "<title>Calabash</title>"
                "</head> "
                "<body>"
                "<h1 style=\"text-align: center;\">404 Page Not Found</h1>"
                "<p style=\"text-align: center;\">request not matched</p>"
                "</body>"
                "</html>";
  BodyData(body);
  SetType(".html");
  set_code(404);
}

void Response::BodyData(const string &dat) {
  body_data_ = dat;
}

void Response::BodyData(const filesystem::path &path) {
  ifstream fs(path, ios::in);
  body_data_ = {std::istreambuf_iterator<char>(fs),
                std::istreambuf_iterator<char>()};
}

void Response::BodyDataStream(const filesystem::path &path) {
  data_reader_.SetFileStream(path);
}

void Response::BodyDataStream(ifstream &&stream) {
  data_reader_.SetFileStream(std::move(stream));
}

void Response::AutoFileData(const filesystem::path &path) {
  ifstream ifs(path);
  ifs.seekg(0, istream::end);
  size_t total_length = ifs.tellg();
  ifs.seekg(0, istream::beg);
  if (total_length > 1024 * 1024) {
    BodyDataStream(std::move(ifs));
  } else {
    ifs.close();
    BodyData(path);
  }
}

string Response::MakeHttpHead(size_t size) {
  ostringstream os;
  os << "HTTP/1.1 " << code_ << " " <<
     (kHttpCodeWithInfo.find(code_) == kHttpCodeWithInfo.end() ? " " : kHttpCodeWithInfo[code_]) << "\r\n";
  for (auto &th : headers_) {
    os << th.first << ": " << th.second << "\r\n";
  }
  os << "Content-Length: " << size << "\r\n\r\n";
  return os.str();
}

void Response::AutoSetType(const filesystem::path &path) {
  string extension = path.extension();
  if (kHttpExtensionWithContentType.find(extension) != kHttpExtensionWithContentType.end()) {
    SetType(kHttpExtensionWithContentType[extension]);
  } else {
    SetType("application/octet-stream");
  }
}

void Response::SetHeader(const string &k, const string &v) {
  for (auto &th : headers_) {
    if (th.first == k) {
      th.second = v;
      return;
    }
  }
  headers_.emplace_back(k, v);
}

Response &Response::operator=(Response &&response) noexcept {
  code_ = response.code_;
  data_reader_ = std::move(response.data_reader_);
  body_data_ = std::move(response.body_data_);
  headers_ = std::move(response.headers_);
  return *this;
}

Response::Response(Response &&response) noexcept {
  code_ = response.code_;
  data_reader_ = std::move(response.data_reader_);
  body_data_ = std::move(response.body_data_);
  headers_ = std::move(response.headers_);
}

