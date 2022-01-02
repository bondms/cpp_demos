// Copyright 2021 Mark Bond

#pragma once

#include <iostream>

#include <asio.hpp>

using std::chrono_literals::operator""s;

class CountdownTimer {
  asio::steady_timer timer_;

  int value_{};

  template <typename WaitHandler>
  void on_timer(WaitHandler &&handler, const asio::error_code &error) {
    if (!error) {
      --value_;
      std::cout << "Countdown: " << value_ << std::endl;
      handler(value_);
      if (0 == value_) {
        return;
      }

      timer_.expires_after(1s);
      timer_.async_wait([this, handler](const asio::error_code &next_error) {
        on_timer(handler, next_error);
      });
    }

    std::cerr << "Error waiting for timer, value: " << error.value()
              << std::endl;
  }

public:
  CountdownTimer(asio::io_context &io_context) : timer_{io_context} {}

  template <typename WaitHandler>
  void initiate(int start_from, WaitHandler handler) {
    value_ = start_from;

    timer_.expires_after(1s);
    timer_.async_wait([this, handler](const asio::error_code &error) {
      on_timer(handler, error);
    });
  }
};
