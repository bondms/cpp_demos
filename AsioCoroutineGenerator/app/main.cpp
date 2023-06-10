// Copyright 2023 Mark Bond
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio.hpp>
#include <asio/experimental/coro.hpp>

#include <cstdlib>
#include <iostream>
#include <random>

asio::experimental::coro<int> randomGenerator(asio::io_context&, int min,
                                              int max) {
  std::random_device rd{};
  std::default_random_engine re{rd()};
  std::uniform_int_distribution<int> uid{min, max};

  while (true) {
    co_yield uid(rd);
  }
}

template <typename G>
asio::experimental::coro<int> countedGenerator(asio::io_context&,
                                               G &generator, int count) {
  auto it = generator.begin();
  for (auto i = 0; i < count; ++i) {
    co_yield *it;
    ++it;
  }
}

template <typename G>
asio::experimental::coro<int> oddifier(asio::io_context&, G &generator) {
  for (const auto &i : generator) {
    co_yield (i % 2 == 0) ? i + 1 : i;
  }
}

int main() {
  std::cout << "Starting..." << std::endl;

  try {
    asio::io_context io_context{};

    auto rg = randomGenerator(io_context, 0, 99);
    // auto o = oddifier(io_context, rg);
    // for ( const auto & i :
    //     countedGenerator<std::experimental::generator<int>>(o, 20) )
    // {
    //     std::cout << i << '\n';
    // }

    // co_spawn(io_context, listen(acceptor, target_endpoint), detached);
    io_context.run();

    std::cout << "Clean shutdown." << std::endl;

    return EXIT_SUCCESS;
  } catch (const std::exception &e) {
    std::cerr << "Main exception: " << e.what() << std::endl;
  }

  return EXIT_FAILURE;
}
