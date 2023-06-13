// Copyright 2023 Mark Bond

#pragma once

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
