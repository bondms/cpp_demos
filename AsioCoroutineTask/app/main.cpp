// Copyright 2023 Mark Bond
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>

#include <asio.hpp>
#include <asio/experimental/coro.hpp>

using namespace std::chrono_literals;

template <typename T>
asio::awaitable<T> delayedTask(asio::io_context &io_context,
                               asio::steady_timer::duration delay, T result) {
  asio::steady_timer timer{io_context};
  timer.expires_after(delay);
  co_await timer.async_wait(asio::use_awaitable);
  co_return result;
}

int main() {
  std::cout << "Starting..." << std::endl;

  try {
    asio::io_context io_context{};

    for (auto i = 1; i < 4; ++i) {
      co_spawn(io_context, delayedTask(io_context, std::chrono::seconds{i}, i),
               [](std::exception_ptr, int result) {
                 std::cout << result << std::endl;
               });
    }

    io_context.run();

    std::cout << "Clean shutdown." << std::endl;

    return EXIT_SUCCESS;
  } catch (const std::exception &e) {
    std::cerr << "Main exception: " << e.what() << std::endl;
  }

  return EXIT_FAILURE;
}
