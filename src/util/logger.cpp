#include "util/logger.h"

#include "iostream"

using namespace calabash;
using namespace std;

const char Logger::log_base_dir_[] = "./log/";

void Logger::Log(const string &s) {
  auto auto_mutex = lock_guard(re_mutex);
  if (today_ != GetNowTime()->tm_mday) {
    today_ = GetNowTime()->tm_mday;
    output_.close();
#ifdef LOGGER_CLEAR_FILE_AT_START
    output_.open(GetFileNameByDate(), ios::out);
    SYSTEM("defined LOGGER_CLEAR_FILE_AT_START");
#else
    output_.open(GetFileNameByDate(), ios::app | ios::out);
#endif
  }
  if (output_.bad()) {
    std::cerr << GetTimeStr() << " output stream bad, Log failed!" << endl;
    return;
  }
  output_ << GetTimeStr() << " " << s << endl;
}

Logger::~Logger() {
  if (output_.is_open())
    output_.close();
}

Logger *Logger::Instance() {
  static Logger *instance{};
  static std::mutex mutex_tmp;
  auto auto_mutex = lock_guard(mutex_tmp);
  if (instance == nullptr) {
    instance = new Logger();
  }
  return instance;
}


