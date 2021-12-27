// Copyright 2021 Mark Bond

#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

namespace ConsumerProducer {

template <typename Item> class Consumer {
  const std::function<void(const Item &item)> process_item_function_{};
  std::mutex mutex_{};
  std::condition_variable condition_variable_{};
  std::queue<Item> queue_{};
  bool quit_{false};
  std::thread thread_{};

public:
  Consumer() = default;

  explicit Consumer(std::function<void(const Item &item)> process_item_function)
      : process_item_function_{std::move(process_item_function)} {
    thread_ = std::thread{&Consumer::thread_function, this};
  }

  Consumer(const Consumer &) = delete;
  Consumer &operator=(const Consumer &) = delete;

  Consumer(Consumer &&) = delete;
  Consumer &operator=(Consumer &&) = delete;

  ~Consumer() {
    if (!thread_.joinable()) {
      return;
    }

    {
      std::lock_guard<std::mutex> lock{mutex_};
      quit_ = true;
    }

    condition_variable_.notify_one();
    thread_.join();
  }

public:
  void push(const Item &item) {
    {
      std::lock_guard<std::mutex> lock{mutex_};
      queue_.push(item);
    }
    condition_variable_.notify_one();
  }

private:
  void thread_function() noexcept {
    while (true) {
      Item item{};

      {
        std::unique_lock<std::mutex> lock{mutex_};
        condition_variable_.wait(lock,
                                 [this] { return quit_ || !queue_.empty(); });

        if (quit_) {
          return;
        }

        item = std::move(queue_.front());
        queue_.pop();
      }

      process_item_function_(item);
    }
  }
};

} // namespace ConsumerProducer
