// Copyright 2023 Mark Bond
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <asio.hpp>
#include <asio/experimental/coro.hpp>

template <typename T>
asio::experimental::generator<T>
oddifier(asio::io_context &, asio::experimental::generator<T> &generator) {
  while (auto n = co_await generator) {
    // clang-format off
    co_yield (*n % 2 == 0) ? *n + 1 : *n;
    // clang-format on
  }
}
