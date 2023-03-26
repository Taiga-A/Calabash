//
// Created by Taiga on 2023/3/26.
//

#ifndef CALABASH_SRC_INCLUDE_APP_EXCEPTION_H_
#define CALABASH_SRC_INCLUDE_APP_EXCEPTION_H_

#include "exception"
#include "string"

#include "nlohmann/json.hpp"

class HttpException : public std::exception {
 public:
  explicit HttpException(int base_code);
  explicit HttpException(const nlohmann::json &body);
  explicit HttpException(int base_code, const std::string &msg);
  explicit HttpException(int base_code, int code, const std::string &msg);
  explicit HttpException(int base_code, int code, std::string msg, nlohmann::json body);

  inline nlohmann::json &body() { return body_; };
  [[nodiscard]] inline const nlohmann::json &body() const { return body_; };
  [[nodiscard]] inline int code() const { return code_; };
  [[nodiscard]] inline int base_code() const { return base_code_; };
  [[nodiscard]] inline const std::string &msg() const { return msg_; };
  [[nodiscard]] const char *what() const noexcept override { return msg_.c_str(); }

 protected:
  int code_;
  int base_code_;
  std::string msg_;
  nlohmann::json body_;
};

#endif //CALABASH_SRC_INCLUDE_APP_EXCEPTION_H_
