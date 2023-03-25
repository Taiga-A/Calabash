//
// Created by Taiga on 2023/3/25.
//

#include "app/token.h"

#include <iomanip>

#include "util/base64.h"
#include "util/md5.h"

using namespace std;
using namespace calabash;

Token::Token(const string &token, const string &key) {
  string pass = Base64::Decode(token);
  string str;
  size_t i = 0;
  for (char th : pass) {
    str.push_back(static_cast<char>(th ^ key[i]));
    i = (i + 1) % key.size();
  }
  if (str.size() <= 32) return;

  if (string(str.begin(), str.begin() + 32) != MD5().Encode(string(str.begin() + 32, str.end()))) return;

  i = 32;
  while (i < str.size() && str[i] != '|') i++;
  if (i == str.size()) return;
  this->user_id_ = string(str.begin() + 32, str.begin() + static_cast<long>(i++));
  string tmp = string(str.begin() + static_cast<long>(i), str.end());
  stringstream ss(tmp);
  ::tm time_struct {};
  ss >> std::get_time(&time_struct, "%Y-%m-%d %H:%M:%S");
  this->end_time_ = chrono::system_clock::from_time_t(::mktime(&time_struct));
  this->is_in_time_ = this->end_time_ >= chrono::system_clock::now();
  this->is_useful_ = true;
}

Token::Token(const string &user_id, std::chrono::seconds effective_time) {
  this->user_id_ = user_id;
  this->end_time_ = chrono::system_clock::now() + effective_time;
  this->is_in_time_ = this->end_time_ >= chrono::system_clock::now();
  this->is_useful_ = true;
}

string Token::Encode(const string &key) {
  string val = ToString();
  size_t i = 0;
  string pass;
  for (char th : val) {
    pass.push_back(static_cast<char>(th ^ key[i]));
    i = (i + 1) % key.size();
  }
  return Base64::Encode(pass);
}

std::string Token::ToString() {
  if(!is_useful_) return {};
  auto time = chrono::system_clock::to_time_t(end_time_);
  auto time_stream = std::put_time(::localtime(&time), "|%Y-%m-%d %H:%M:%S");
  stringstream ss;
  ss << time_stream;
  string payload = user_id_ + ss.str();
  return MD5().Encode(payload) + payload;
}