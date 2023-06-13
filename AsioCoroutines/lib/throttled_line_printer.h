// Copyright 2023 Mark Bond

#pragma once

#include <asio.hpp>

template <typename G>
asio::awaitable<void> throttledLinePrinter(asio::io_context &io_context,
                                           G &generator,
                                           asio::steady_timer::duration delay) {
  asio::steady_timer timer{io_context};
  while (auto n = co_await generator.async_resume(asio::use_awaitable)) {
    std::cout << *n << '\n';
    timer.expires_after(delay);
    co_await timer.async_wait(asio::use_awaitable);
  }
}
