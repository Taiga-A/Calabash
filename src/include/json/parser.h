#pragma once

#include <string>
using namespace std;

#include <json/json.h>

namespace calabash {

class Parser {
 public:
  Parser() = default;
  ~Parser() = default;

  void load(const string &str);
  Json parse();

 private:
  void skip_white_space();
  char get_next_token();

  Json parse_null();
  Json parse_bool();
  Json parse_number();
  string parse_string();
  Json parse_array();
  Json parse_object();

  static bool in_range(int x, int lower, int upper) {
    return (x >= lower && x <= upper);
  }

 private:
  string m_str{};
  size_t m_idx{};
};

} // namespace calabash

