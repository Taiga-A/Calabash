#include "util/logger.h"

#include "iostream"

using namespace calabash;
using namespace std;

const char Logger::log_base_dir_[] = "./log/";

Logger::Logger() {
  today_ = GetNowTime()->tm_mday;
#ifdef LOGGER_CLEAR_FILE_AT_START
  output_.open(GetFileNameByDate(), ios::out);
  output_ << "Clear this file before start,because defined LOGGER_CLEAR_FILE_AT_START" << endl;
#else
  output_.open(GetFileNameByDate(), ios::app | ios::out);
#endif
}

void Logger::Log(const string &s) {
  auto auto_mutex = lock_guard(mutex_);
  if (today_ != GetNowTime()->tm_mday) {
    output_.close();
    output_.open(GetFileNameByDate(), ios::app | ios::out);
  }
  if (output_.bad()) {
    std::cerr << GetTimeStr() << " output stream bad, Log failed!" << endl;
    return;
  }
  output_ << GetTimeStr() << " " << s << endl;
}

Logger::~Logger() {
  output_.close();
}

Logger *Logger::Instance() {
  static Logger *instance{};
  static std::mutex mutex_tmp;
  auto auto_mutex = lock_guard(mutex_tmp);
  if (instance == nullptr)
    instance = new Logger();
  return instance;
}


