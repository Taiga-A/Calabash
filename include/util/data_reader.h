//
// Created by Taiga on 2023/3/17.
//

#ifndef CALABASH_SRC_UTIL_DATA_READER_H_
#define CALABASH_SRC_UTIL_DATA_READER_H_

#include <fstream>
#include <filesystem>
#include <functional>

#include "util/logger.h"

namespace calabash {
class DataReader {
 public:
  using buf_read_fun = std::function<size_t(const char *, size_t size)>;

  DataReader() = default;
  DataReader(DataReader &) = delete;
  DataReader(DataReader &&) noexcept;

  DataReader &operator=(DataReader &) = delete;
  DataReader &operator=(DataReader &&) noexcept;

  void SetFileStream(const std::filesystem::path &path);
  void SetFileStream(std::ifstream &&input_stream);
  void SetBeginData(const std::string &data);

  inline void ClearFileStream();

  inline size_t GetStreamSize() const { return stream_size_; };
  inline size_t Size() const { return stream_size_ + begin_data_.size(); };
  inline std::string &GetBeginData() { return begin_data_; };

  void BufferRead(const buf_read_fun &func);

 private:
  static constexpr size_t kReadBufSize = 4_KB;
  std::ifstream input_file_stream_{};
  std::string begin_data_{};
  size_t stream_size_{};
};

void DataReader::ClearFileStream() {
  this->stream_size_ = 0;
  this->input_file_stream_.close();
}

} // namespace calabash

#endif //CALABASH_SRC_UTIL_DATA_READER_H_
