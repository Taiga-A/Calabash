#pragma once

#include <vector>

#include "thread"
#include "memory"
#include "mutex"

namespace calabash {

template<typename T>
class [[maybe_unused]] ObjectPool {
 private:
  using shared_ptr = std::shared_ptr<T>;

  explicit ObjectPool(size_t block_size = 40) : block_size_(block_size), max_size_(-1) {
    AllocNewBlock();
  }

  ~ObjectPool() {
    for (char *th : all_memory_)
      delete[] th;
  }

 public:
  ObjectPool(ObjectPool<T> &) = delete;

  ObjectPool<T> &operator=(ObjectPool<T> &) = delete;

  static ObjectPool<T> *Instance() {
    static ObjectPool<T> *ptr{};
    static std::mutex mutex{};
    auto auto_mutex = std::lock_guard(mutex);
    if (ptr == nullptr)
      ptr = new ObjectPool<T>();
    return ptr;
  }

  void SetBlockSize(size_t new_block_size) {
    auto auto_mutex = std::lock_guard(mutex_);
    block_size_ = new_block_size;
  }

  void SetMaxSize(size_t new_size) {
    auto auto_mutex = std::lock_guard(mutex_);
    max_size_ = new_size;
  }

  template<typename ...Args>
  T *Allocate(Args &&... args) {
    auto auto_mutex = std::lock_guard(mutex_);
    if (max_size_ != -1 && now_size_ == max_size_) {
      return nullptr;
    }
    if (idle_objs_.empty())
      AllocNewBlock();
    T *ret = idle_objs_.back();
    idle_objs_.pop_back();
    now_size_++;
    return new(ret) T(args...);
  }

  template<typename ...Args>
  shared_ptr AllocateShared(Args &&... args) {
    T *temp = Allocate(args...);
    return shared_ptr(temp, [&](T *p) { Delete(p); });
  }

  void Delete(T *obj_ptr) {
    auto auto_mutex = std::lock_guard(mutex_);
    now_size_--;
    obj_ptr->~T();
    idle_objs_.push_back(obj_ptr);
  }

 private:
  void AllocNewBlock() {
    auto auto_mutex = std::lock_guard(mutex_);
    char *temp = new char[sizeof(T) * block_size_];
    all_memory_.push_back(temp);
    for (size_t i = 0; i < block_size_; i++) {
      idle_objs_.push_back(reinterpret_cast<T *>(temp + i * (sizeof(T))));
    }
  };

 private:
  std::vector<char *> all_memory_;
  std::vector<T *> idle_objs_;
  size_t block_size_;
  std::mutex mutex_;
  size_t max_size_;
  size_t now_size_{};
};

}
