#pragma once

#include <queue>
#include <condition_variable>
#include <limits>

template<typename T>
class ConcurrentQueue
{
 public:
  explicit ConcurrentQueue(size_t max_size = std::numeric_limits<size_t>::max()) : max_size_{max_size}
  {}

  ConcurrentQueue(ConcurrentQueue const&) = delete;
  ConcurrentQueue& operator=(ConcurrentQueue const&) = delete;

  T Pop()
  {
    std::unique_lock lock(mtx_);
    is_not_empty_.wait(lock, [this]() { return !queue_.empty(); });

    T value = queue_.front();
    queue_.pop();

    lock.unlock();
    is_not_full_.notify_one();

    return value;
  }

  void Push(T value)
  {
    std::unique_lock lock(mtx_);
    is_not_full_.wait(lock, [this]() { return queue_.size() < max_size_; });

    queue_.push(value);
    lock.unlock();
    is_not_empty_.notify_one();
  }

 private:
  size_t max_size_;

  mutable std::mutex      mtx_;
  std::condition_variable is_not_full_;
  std::condition_variable is_not_empty_;

  std::queue<T> queue_;
};

