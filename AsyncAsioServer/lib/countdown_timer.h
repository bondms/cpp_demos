// Copyright 2021 Mark Bond

#pragma once

#include <chrono>
#include <tuple>
#include <utility>

#include <asio.hpp>

class CountdownTimer {
  asio::steady_timer timer_;

  bool aborted_{};
  std::chrono::milliseconds interval_{};
  int value_{};

  template <typename WaitHandler>
  void on_timer(WaitHandler &&handler, const asio::error_code &error) {
    if (aborted_) {
      return;
    }

    if (error) {
      throw std::runtime_error{"Error waiting for timer, value: " +
                               std::to_string(error.value())};
    }

    --value_;
    handler(value_);
    if (0 == value_) {
      return;
    }

    timer_.expires_after(interval_);
    timer_.async_wait([this, capture = std::make_tuple(std::move(handler))](
                          const asio::error_code &next_error) mutable {
      on_timer(std::get<0>(capture), next_error);
    });
  }

public:
  explicit CountdownTimer(asio::io_context &io_context) : timer_{io_context} {}

  template <typename WaitHandler>
  void initiate(int start_from, std::chrono::milliseconds interval,
                WaitHandler &&handler) {
    if (start_from <= 0) {
      return;
    }

    aborted_ = false;
    interval_ = interval;
    value_ = start_from;

    timer_.expires_after(interval_);
    timer_.async_wait([this, capture = std::make_tuple(std::move(handler))](
                          const asio::error_code &error) mutable {
      on_timer(std::get<0>(capture), error);
    });
  }

  void abort() {
    aborted_ = true;
    timer_.cancel();
  }
};
