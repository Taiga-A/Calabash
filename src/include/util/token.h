//
// Created by Taiga on 2023/3/25.
//

#ifndef CALABASH_SRC_INCLUDE_APP_TOKEN_H_
#define CALABASH_SRC_INCLUDE_APP_TOKEN_H_

#include <chrono>
#include <string>

namespace calabash {

class Token {
  using token_time_t = std::chrono::time_point<std::chrono::system_clock>;

 public:
  Token() = default;
  Token(const std::string &token, const std::string &key);
  Token(const std::string & user_id, std::chrono::seconds effective_time);

  std::string Encode(const std::string &key);
  std::string ToString();

  [[nodiscard]] inline bool useful() const { return is_useful_; };
  [[nodiscard]] inline bool in_time() const { return is_in_time_; };
  [[nodiscard]] inline std::string get_user() const { return user_id_; };

 private:
  bool is_useful_ = false;
  bool is_in_time_ = false;
  std::string user_id_;
  token_time_t end_time_;
};

} // namespace calabash


#endif //CALABASH_SRC_INCLUDE_APP_TOKEN_H_
