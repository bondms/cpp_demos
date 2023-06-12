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

using std::chrono_literals::operator""s;

template <typename T>
asio::experimental::generator<T> randomGenerator(asio::io_context &, T min,
                                                 T max) {
  std::random_device rd{};
  std::default_random_engine re{rd()};
  std::uniform_int_distribution<T> uid{min, max};

  while (true) {
    co_yield uid(rd);
  }
}

template <typename T>
asio::experimental::generator<T>
countedGenerator(asio::io_context &,
                 asio::experimental::generator<T> &generator,
                 std::size_t count) {
  while (count > 0) {
    --count;
    auto n = co_await generator;
    if (!n) {
      co_return;
    }
    co_yield *n;
  }
}

template <typename T>
asio::experimental::generator<T>
oddifier(asio::io_context &, asio::experimental::generator<T> &generator) {
  while (auto n = co_await generator) {
    // clang-format off
    co_yield (*n % 2 == 0) ? *n + 1 : *n;
    // clang-format on
  }
}

template <typename G>
asio::awaitable<void> throttledLinePrinter(asio::io_context &io_context,
                                           G &generator,
                                           asio::steady_timer::duration delay) {
  while (auto n = co_await generator.async_resume(asio::use_awaitable)) {
    std::cout << *n << '\n';
    asio::steady_timer timer{io_context};
    timer.expires_after(delay);
    co_await timer.async_wait(asio::use_awaitable);
  }
}

int main() {
  std::cout << "Starting..." << std::endl;

  try {
    asio::io_context io_context{};

    auto rg = randomGenerator(io_context, 0, 99);
    auto cg = countedGenerator(io_context, rg, 20);
    auto o = oddifier(io_context, cg);
    auto fast_lp = throttledLinePrinter(io_context, o, 1s);
    auto slow_lp = throttledLinePrinter(io_context, o, 3s);

    co_spawn(io_context, std::move(slow_lp), asio::detached);
    co_spawn(io_context, std::move(fast_lp), asio::detached);

    io_context.run();

    std::cout << "Clean shutdown." << std::endl;

    return EXIT_SUCCESS;
  } catch (const std::exception &e) {
    std::cerr << "Main exception: " << e.what() << std::endl;
  }

  return EXIT_FAILURE;
}
