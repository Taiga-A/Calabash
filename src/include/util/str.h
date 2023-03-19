#pragma once

#include <sstream>
#include <vector>
#include <string>

namespace calabash {

class Str {
 public:
  static std::string to_lower(const std::string &input);
  static std::string to_upper(const std::string &input);

  static char to_char(const std::string &input);
  static short to_short(const std::string &input);
  static int to_int(const std::string &input);
  static long to_long(const std::string &input);
  static float to_float(const std::string &input);
  static double to_double(const std::string &input);

  static std::string trim_start(const std::string &input);
  static std::string trim_start(const std::string &input, char trim);
  static std::string trim_start(const std::string &input, const char *trims);

  static std::string trim_end(const std::string &input);
  static std::string trim_end(const std::string &input, char trim);
  static std::string trim_end(const std::string &input, const char *trims);

  static std::string trim(const std::string &input);
  static std::string trim(const std::string &input, char trim);
  static std::string trim(const std::string &input, const char *trims);

  static void split(std::vector<std::string> &output, const std::string &input);
  static void split(std::vector<std::string> &output, const std::string &input, char separator);
  static void split(std::vector<std::string> &output, const std::string &input, const std::string &separators);

  static std::string join(std::vector<std::string> &input);
  static std::string join(std::vector<std::string> &input, char separator);
  static std::string join(std::vector<std::string> &input, const char *separators);

  static std::string capitalize(const std::string &input);

  static bool is_numeric(const std::string &input);

  static std::string UrlEncode(const std::string &input);
  static std::string UrlDecode(const std::string &input);
  static char UrlCodeGetHEX(char l, char r);

 private:
  static const char kNumMap[];
  static const char kAsciiMap[];
};

}

