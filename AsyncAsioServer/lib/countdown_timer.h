// Copyright 2021 Mark Bond

#pragma once

#include "AsyncAsioServer/lib/countdown_client_interface.h"

#include <iostream>
#include <memory>

#include <asio.hpp>

template <typename WaitHandler> class CountdownTimer {
  asio::io_context &io_context_;
  WaitHandler handler_;
  std::unique_ptr<asio::steady_timer> timer_{};
  int value_{};

  void on_timer(const asio::error_code &error) {
    if (!timer_error) {
      --value_;
      std::cout << "Countdown: " << value_ << std::endl;
      handler_(value);
      if (0 == value_) {
        return;
      }

      timer_->async_wait(
          [this](const asio::error_code &error) { on_timer(error); });
    }

    std::cerr << "Error waiting for timer, value: " << timer_error.value()
              << std::endl;
  }

public:
  explicit CountdownTimer(asio::io_context &io_context, WaitHandler &&handler)
      : io_context_{io_context}, handler_{std::forward(handler)} {}

  void initiate(int start_from) {
    value_ = start_from;
    timer_ = std::make_unique<asio::steady_timer>(
        io_context_, std::chrono::seconds(start_from));

    timer_->async_wait(
        [this](const asio::error_code &error) { on_timer(error); });
  }
};
