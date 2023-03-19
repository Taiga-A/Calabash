#pragma once

#include <string>
#include <ostream>
#include <list>
#include <map>

namespace calabash {

class Json {
 public:
  enum Type {
    json_null = 0,  // null value
    json_bool,      // bool value
    json_int,       // integer value
    json_double,    // double value
    json_string,    // string value
    json_array,     // array value
    json_object     // object value
  };

  Json();
  explicit Json(Type type);
  explicit Json(bool value);
  explicit Json(int value);
  explicit Json(double value);
  explicit Json(const char *value);
  explicit Json(const std::string &value);
  Json(const Json &other);
  ~Json() = default;

  Type type() const;

  bool isNull() const;
  bool isBool() const;
  bool isInt() const;
  bool isDouble() const;
  bool isString() const;
  bool isArray() const;
  bool isObject() const;

  bool asBool() const;
  int asInt() const;
  double asDouble() const;
  std::string asString() const;

  // number of values in array or object
  int size() const;

  // return true if empty array, empty object, or null, otherwise, false.
  bool empty() const;

  void clear();

  bool has(int index) const;
  bool has(const char *key) const;
  bool has(const std::string &key) const;

  Json get(int index) const;
  Json get(const char *key) const;
  Json get(const std::string &key) const;

  void remove(int index);
  void remove(const char *key);
  void remove(const std::string &key);

  // append value to array at the end.
  void append(const Json &value);

  Json &operator=(const Json &other);
  bool operator==(const Json &other);
  bool operator!=(const Json &other);

  Json &operator[](int index);
  Json &operator[](const char *key);
  Json &operator[](const std::string &key);

  friend std::ostream &operator<<(std::ostream &os, const Json &json) {
    os << json.str();
    return os;
  }

  operator bool();
  operator int();
  operator double();
  operator std::string();

  void parse(const std::string &str);
  std::string str() const;

  typedef std::list<Json>::iterator iterator;
  iterator begin() const {
    return (m_value.m_array)->begin();
  }
  iterator end() const {
    return (m_value.m_array)->end();
  }

 private:
  void copy(const Json &other);

 private:
  union Value {
    bool m_bool;
    int m_int;
    double m_double;
    std::string *m_string;
    std::list<Json> *m_array;
    std::map<std::string, Json> *m_object;
  };

  Type m_type;
  Value m_value{};
};

}