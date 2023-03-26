//
// Created by Taiga on 2023/3/16.
//

#ifndef CALABASH_INCLUDE_WEB_REQUEST_MAKER_H_
#define CALABASH_INCLUDE_WEB_REQUEST_MAKER_H_

#include "util/logger.h"
#include "web/request.h"
#include "socket/socket.h"

namespace calabash {

class RequestMaker {
 public:
  static bool Parse(Socket *socket, Request *req);

 private:
  static size_t ParseHttpHeader(const char *buf, Request *req);
  static void SaveMultipartFile(const char *buf, const char *start, size_t len, Request *req);

  inline static const char *GetNextWord(const char *buf, std::string *str, const char *end = nullptr);
  inline static const char *GetNextLine(const char *buf, std::string *str);

  inline static std::vector<std::string> Split(std::string &str, char ch);

  inline static bool TestContentTypeMultipart(const std::string &str);
  inline static bool TestContentTypeJson(const std::string &str);

  static std::string UrlEncode(const std::string &url);
  static std::string UrlDecode(const std::string &url);
  inline static char UrlCodeGetHEX(char l, char r);

  static std::string kNumMap;
  static std::string kAsciiMap;

  static constexpr size_t kReqBufSize = 512_KB;
};

const char *RequestMaker::GetNextWord(const char *buf, std::string *str, const char *end) {
  const char *ptr = buf;
  while (buf != end && (*buf == ' ' || *buf == '\r' || *buf == '\n' || *buf == '\t')) buf++;
  while (ptr != end && *ptr != ' ' && *ptr != '\r' && *ptr != '\n' && *ptr != '\t') ptr++;
  *str = std::string(buf, ptr - buf);
  while (ptr != end && (*ptr == ' ' || *ptr == '\r' || *ptr == '\n' || *ptr == '\t')) ptr++;
  return ptr;
}

const char *RequestMaker::GetNextLine(const char *buf, std::string *str) {
  const char *ptr = buf;
  while (*ptr != '\r' && *ptr != '\n') ptr++;
  *str = std::string(buf, ptr - buf);
  if (*ptr == '\r') ptr++;
  return ptr + 1;
}

std::vector<std::string> RequestMaker::Split(std::string &str, char ch) {
  std::vector<std::string> res;
  int bg = 0, ed = 0;
  while (ed < str.size()) {
    while (ed < str.size() && str[ed] != ch) ed++;
    res.emplace_back(str.c_str() + bg, ed - bg);
    bg = ++ed;
  }
  return res;
}

bool RequestMaker::TestContentTypeMultipart(const std::string &str) {
  constexpr char multipart_text[] = "multipart";
  for (int i = 0; i < sizeof(multipart_text) - 1; i++) {
    if (multipart_text[i] != str[i]) {
      return false;
    }
  }
  return true;
}

bool RequestMaker::TestContentTypeJson(const std::string &str) {
  constexpr char multipart_text[] = "application/json";
  for (int i = 0; i < sizeof(multipart_text) - 1; i++) {
    if (multipart_text[i] != str[i]) {
      return false;
    }
  }
  return true;
}

char RequestMaker::UrlCodeGetHEX(char l, char r) {
  unsigned char ll = std::find(std::begin(kNumMap), std::end(kNumMap), l) - std::begin(kNumMap);
  unsigned char rr = std::find(std::begin(kNumMap), std::end(kNumMap), r) - std::begin(kNumMap);
  return static_cast<char>((ll << 4) + rr);
}

}

#endif //CALABASH_INCLUDE_WEB_REQUEST_MAKER_H_
