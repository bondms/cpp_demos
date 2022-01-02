// Copyright 2021 Mark Bond

#pragma once

Countdown::Countdown(CountdownClientInterface & countdown_client)
{
}

void Countdown::initiate(int start_from) {
  timer_ = std::make_unique<asio::steady_timer>(io_context_, std::chrono::seconds(start_from));

  timer_->async_wait([](const asio::error_code &timer_error) {
    if (!timer_error) {
      std::cout << "Timer expired; closing connection." << std::endl;
      return;
    }

    std::cerr << "Error waiting for timer, value: " << timer_error.value()
              << std::endl;
  });
}
