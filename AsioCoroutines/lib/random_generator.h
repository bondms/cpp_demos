// Copyright 2023 Mark Bond

#pragma once

#include <random>

#include <asio.hpp>
#include <asio/experimental/coro.hpp>

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
