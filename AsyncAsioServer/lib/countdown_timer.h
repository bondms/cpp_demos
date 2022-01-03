// Copyright 2021 Mark Bond

#pragma once

#include <chrono>
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
    timer_.async_wait([this, handler = std::forward<WaitHandler>(handler)](
                          const asio::error_code &next_error) {
      on_timer(std::forward<WaitHandler>(handler), next_error);
    });
  }

public:
  explicit CountdownTimer(asio::io_context &io_context) : timer_{io_context} {}

  template <typename WaitHandler>
  void initiate(int start_from, std::chrono::milliseconds interval,
                WaitHandler &&handler) {
    aborted_ = false;
    interval_ = interval;
    value_ = start_from;

    struct {
      WaitHandler handler;
    } capture{std::forward<WaitHandler>(handler)};

    timer_.expires_after(interval_);
    timer_.async_wait(
        [this, capture = std::move(capture)](const asio::error_code &error) {
          on_timer(capture.handler, error);
        });
  }

  void abort() {
    aborted_ = true;
    timer_.cancel();
  }
};
