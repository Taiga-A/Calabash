#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <iomanip>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <vector>

// TODO( printf Log.. )


#define SYSTEM(s) calabash::Logger::Instance()->System(s)
#define INFO(s) calabash::Logger::Instance()->Info(s)
#define WARNING(s) calabash::Logger::Instance()->Warning(s)
#define ERROR(arg...) calabash::Logger::Instance()->Error(arg)
#define CONSOLE(s) calabash::Logger::Console(s, __FILE__, __LINE__)

#define ASSERT(t) if (!(t)) {                                                 \
  std::cerr << "assert false: File:" << __FILE__ << " Line:" << __LINE__ ;    \
  ERROR(std::string("assert false: File: ") + __FILE__ + " " + std::to_string(__LINE__), -1); \
                                                                              \
}

// #define LOGGER_CLEAR_FILE_AT_START

namespace calabash {

constexpr size_t operator ""_GB(unsigned long long n) { return n * 1024 * 1024 * 1024; }
constexpr size_t operator ""_MB(unsigned long long n) { return n * 1024 * 1024; }
constexpr size_t operator ""_KB(unsigned long long n) { return n * 1024; }
constexpr size_t operator ""_B(unsigned long long n) { return n; }
constexpr size_t operator ""_b(unsigned long long n) { return n / 8; }

class Logger {
 public:
  static Logger *Instance();

  inline void Info(const std::string &s);
  inline void System(const std::string &s);
  inline void Warning(const std::string &s);
  inline void Error(const std::string &s);
  inline void Error(const std::string &s, int code);

  static inline void Console(const std::string &s, const char *file, int line);

 private:
  Logger() = default;
  ~Logger();

  void Log(const std::string &s);
  inline static tm *GetNowTime();
  inline static std::_Put_time<char> GetTimeStr();
  inline static std::string GetFileNameByDate();

 private:
  static const char log_base_dir_[];
  int today_ = -1;
  std::string filename_;
  std::ofstream output_;
  std::mutex mutex_;
  std::recursive_mutex re_mutex;
};

tm *Logger::GetNowTime() {
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  return ::localtime(&time);
}

std::_Put_time<char> Logger::GetTimeStr() {
  return std::put_time(GetNowTime(), "[%Y-%m-%d %X]");
}

std::string Logger::GetFileNameByDate() {
  char buf[20];
  ::strftime(buf, sizeof(buf), "%Y-%m-%d.log", GetNowTime());

  std::filesystem::create_directories(log_base_dir_);
  return {std::string(log_base_dir_) + buf};
}

void Logger::System(const std::string &s) {
  Log("SYSTEM  | " + s);
}

void Logger::Info(const std::string &s) {
  Log("INFO    | " + s);
}

void Logger::Warning(const std::string &s) {
  Log("WARNING | " + s);
}

void Logger::Error(const std::string &s, int code) {
  Log("ERROR   | " + s + " exit: " + std::to_string(code));
  exit(code);
}

void Logger::Error(const std::string &s) {
  Log("ERROR   | " + s);
}

void Logger::Console(const std::string &s, const char *file, int line) {
  printf("INFO %s:%d %s\n", file, line, s.c_str());
}

template<typename T>
void print(const std::vector<T> &arr) {
  for (const T &th : arr) {
    std::cout << th << " ";
  }
  std::cout << std::endl;
}

template<typename T>
void print(const std::vector<std::vector<T>> &arr) {
  int lines = 1;
  for (const std::vector<T> &arr_ : arr) {
    std::cout << lines++ << " : ";
    for (const T &th : arr_) {
      std::cout << th << " ";
    }
    std::cout << std::endl;
  }
}

} // namespace calabash

