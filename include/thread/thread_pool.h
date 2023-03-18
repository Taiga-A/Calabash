#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

namespace calabash {

class ThreadPool {
 public:
  explicit ThreadPool(size_t);
  ~ThreadPool();

  template<class F, class... Args>
  auto Enqueue(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

 private:
  // need to keep track of threads, so we can join them
  std::vector<std::thread> workers_;
  // the task queue
  std::queue<std::function<void()> > tasks_;

  // synchronization
  std::mutex queue_mutex_;
  std::condition_variable condition_;
  bool stop_;
};

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::Enqueue(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
  using return_type = typename std::result_of<F(Args...)>::type;

  auto task = std::make_shared<std::packaged_task<return_type()> >(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...)
  );

  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);

    // don't allow enqueueing after stopping the pool
    if (stop_) {
      throw std::runtime_error("enqueue on stopped ThreadPool");
    }

    tasks_.emplace([task]() { (*task)(); });
  }
  condition_.notify_one();
  return res;
}

}
