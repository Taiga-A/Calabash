
#ifndef CALABASH_INCLUDE_WEB_REQUEST_H_
#define CALABASH_INCLUDE_WEB_REQUEST_H_

#include <string>
#include <map>

namespace calabash {
class Request {
  friend class RequestMaker;
 public:
  Request() = default;
  ~Request() = default;

  [[nodiscard]] inline std::string method() const { return method_; };
  inline void set_from_ip(const std::string &from_ip) { from_ip_ = from_ip; };
  [[nodiscard]] std::string header(const std::string &name) const;
  [[nodiscard]] std::string cookie(const std::string &name) const;
  [[nodiscard]] std::string path() const;
  [[nodiscard]] size_t body_size() const { return body_size_; }
  [[nodiscard]] inline std::string body() const { return body_; };

 private:
  std::string method_;
  std::string url_;
  std::string version_;
  std::string path_;
  std::string body_;
  size_t body_size_{};
  std::string from_ip_;
  std::map<std::string, std::string> params_;
  std::map<std::string, std::string> form_data_;
  std::map<std::string, std::string> headers_;
  std::map<std::string, std::string> cookies_;
};

}

#endif  // CALABASH_INCLUDE_WEB_REQUEST_H_
