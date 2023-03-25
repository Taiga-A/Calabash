//
// Created by Taiga on 2023/3/8.
//

#include "app/student_mange_server.h"
#include "nlohmann/json.hpp"
#include "app/token.h"
#include "util/logger.h"
#include "util/base64.h"
#include "util/md5.h"

using namespace std;
using namespace calabash;
using namespace nlohmann;

string XOR(const string &val, const string &key) {
  size_t i = 0;
  string ret;
  for (char th : val) {
    ret.push_back(static_cast<char>(th ^ key[i]));
    i = (i + 1) % key.size();
  }
  return ret;
}

string Encode(const string &val, const string &key) {
  return Base64::Encode(XOR(val, key));
}

string Decode(const string &val, const string &key) {
  return XOR(Base64::Decode(val), key);
}

int main() {
//  auto server = StudentMangeServer::Instance();
//  server->Init(8080, "test.db");

//  string str = MD5().Encode("wang123456");
//
//  cout << str << endl;



  return 0;
}
