#include <web/request.h>

#include <cstring>
#include <vector>
#include "iostream"

using namespace calabash;
using namespace std;

string Request::header(const string &name) const {
  auto it = headers_.find(name);
  if (it != headers_.end()) {
    return it->second;
  }
  return "";
}

string Request::cookie(const string &name) const {
  auto it = cookies_.find(name);
  if (it != cookies_.end()) {
    return it->second;
  }
  return "";
}

string Request::path() const {
  return path_;
}
