// Copyright 2023 Mark Bond
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio.hpp>
#include <asio/experimental/coro.hpp>

#include <random>

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
