// Copyright 2021 Mark Bond

#pragma once

#include <asio.hpp>

using std::chrono_literals::operator""s;

class CountdownTimer {
  asio::steady_timer timer_;

  int value_{};

  template <typename WaitHandler>
  void on_timer(WaitHandler handler, const asio::error_code &error) {
    if (asio::error::operation_aborted == error.value()) {
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

    timer_.expires_after(1s);
    timer_.async_wait([this, handler](const asio::error_code &next_error) {
      on_timer(handler, next_error);
    });
  }

public:
  CountdownTimer(asio::io_context &io_context) : timer_{io_context} {}

  template <typename WaitHandler> void initiate(WaitHandler handler) {
    value_ = 10;

    timer_.expires_after(1s);
    timer_.async_wait([this, handler](const asio::error_code &error) {
      on_timer(handler, error);
    });
  }

  void abort() { timer_.cancel(); }
};
