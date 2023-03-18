//
// Created by Taiga on 2023/3/17.
//


#include "util/data_reader.h"

using namespace calabash;
using namespace std;

DataReader::DataReader(DataReader &&data_reader) noexcept {
  if (this->input_file_stream_.is_open())
    this->input_file_stream_.close();
  this->input_file_stream_ = std::move(data_reader.input_file_stream_);
  this->begin_data_ = std::move(data_reader.begin_data_);
  this->stream_size_ = data_reader.stream_size_;
}

DataReader &DataReader::operator=(DataReader &&data_reader) noexcept {
  if (this->input_file_stream_.is_open())
    this->input_file_stream_.close();
  this->input_file_stream_ = std::move(data_reader.input_file_stream_);
  this->begin_data_ = std::move(data_reader.begin_data_);
  this->stream_size_ = data_reader.stream_size_;
  return *this;
}

void DataReader::SetFileStream(const filesystem::path &path) {
  input_file_stream_ = ifstream(path, ios::in);
  input_file_stream_.seekg(0, istream::end);
  stream_size_ = input_file_stream_.tellg();
  input_file_stream_.seekg(0, istream::beg);
}

void DataReader::SetFileStream(ifstream &&input_stream) {
  this->input_file_stream_ = std::move(input_stream);
  input_file_stream_.seekg(0, istream::end);
  stream_size_ = input_file_stream_.tellg();
  input_file_stream_.seekg(0, istream::beg);
}

void DataReader::SetBeginData(const string &data) {
  begin_data_ = data;
}

void DataReader::BufferRead(const DataReader::buf_read_fun &func) {
  char buf[kReadBufSize];
  if (!begin_data_.empty()) {
    size_t total_size = begin_data_.size();
    size_t real_read_size = 0;
    while (real_read_size < total_size) {
      size_t th_read_size = func(begin_data_.c_str() + real_read_size, total_size - real_read_size);
      real_read_size += th_read_size;
    }
  }
  size_t real_read_size = 0;
  while (real_read_size < stream_size_) {
    size_t pk_read_size = input_file_stream_.readsome(buf, kReadBufSize);
    size_t pk_real_send = 0;
    while (pk_real_send < pk_read_size) {
      size_t th_send = func(buf + pk_real_send, pk_read_size - pk_real_send);
      pk_real_send += th_send;
    }
    real_read_size += pk_real_send;
  }
  input_file_stream_.seekg(0, istream::beg);
}


