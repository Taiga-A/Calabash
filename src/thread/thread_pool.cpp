#include "thread/thread_pool.h"

using namespace calabash;

// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t threads) : stop_(false) {
  for (size_t i = 0; i < threads; ++i)
    workers_.emplace_back([this] {
                            while (true) {
                              std::function<void()> task;
                              {
                                std::unique_lock<std::mutex> lock(this->queue_mutex_);
                                this->condition_.wait(lock, [this] { return this->stop_ || !this->tasks_.empty(); });
                                if (this->stop_ && this->tasks_.empty()) {
                                  return;
                                }
                                task = std::move(this->tasks_.front());
                                this->tasks_.pop();
                              }
                              task();
                            }
                          }
    );
}

// the destructor joins all threads
ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    stop_ = true;
  }
  condition_.notify_all();
  for (std::thread &worker : workers_) {
    worker.join();
  }
}