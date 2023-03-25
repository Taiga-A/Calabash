#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

#include "util/data_reader.h"

namespace calabash {
class Response {
 public:
  Response() = default;
  Response(Response &&response) noexcept;
  ~Response() = default;

  Response &operator=(Response &&response) noexcept;

  inline int code() const { return code_; };
  inline void set_code(int code) { code_ = code; };
  void page_not_found();

  void SetHeader(const std::string &k, const std::string &v);
  void SetType(const std::string &type);
  void AutoSetType(const std::filesystem::path &path);
  inline void SetHtmlType() { SetType(".html"); };
  inline void SetJsonType() { SetType(".json"); };

  void BodyData(const std::string &dat);
  void BodyData(const std::filesystem::path &path);
  void BodyDataStream(const std::filesystem::path &path);
  void BodyDataStream(std::ifstream &&stream);
  void AutoFileData(const std::filesystem::path &path);

  inline DataReader &GetDataReader();
  std::string MakeHttpHead(size_t size);

 private:
  int code_ = 200;
  DataReader data_reader_;
  std::string body_data_;
  std::vector<std::pair<std::string, std::string>> headers_;
};

inline DataReader &Response::GetDataReader() {
  data_reader_.SetBeginData(
      MakeHttpHead(body_data_.size() + data_reader_.GetStreamSize()) +
          body_data_);
  return data_reader_;
}

}
