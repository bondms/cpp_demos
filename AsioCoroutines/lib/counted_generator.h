// Copyright 2023 Mark Bond
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio.hpp>
#include <asio/experimental/coro.hpp>

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
