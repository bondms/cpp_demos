// Copyright 2021 Mark Bond

#pragma once

#include "AsyncAsioServer/lib/countdown_client_interface.h"

#include <iostream>

#include <asio.hpp>

using std::chrono_literals::operator""s;

class CountdownTimer {
  WaitHandler handler_;

  asio::steady_timer timer_{};
  int value_{};

  void on_timer(const asio::error_code &error) {
    if (!timer_error) {
      --value_;
      std::cout << "Countdown: " << value_ << std::endl;
      handler_(value);
      if (0 == value_) {
        return;
      }

      timer_.expires_after(1s);
      timer_.async_wait(
          [this](const asio::error_code &error) { on_timer(error); });
    }

    std::cerr << "Error waiting for timer, value: " << timer_error.value()
              << std::endl;
  }

public:
  CountdownTimer() {}

  template <typename WaitHandler>
  void initiate(int start_from, WaitHandler &&handler) {
    handler_ = std::forward(handler);
    value_ = start_from;

    timer_.expires_after(1s);
    timer_.async_wait(
        [this](const asio::error_code &error) { on_timer(error); });
  }
};
