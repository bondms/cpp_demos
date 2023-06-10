// Copyright 2023 Mark Bond
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio.hpp>
#include <asio/experimental/coro.hpp>

#include <cstdlib>
#include <iostream>
#include <random>

template <typename T>
asio::experimental::coro<T> randomGenerator(asio::io_context &, T min, T max) {
  std::random_device rd{};
  std::default_random_engine re{rd()};
  std::uniform_int_distribution<T> uid{min, max};

  while (true) {
    co_yield uid(rd);
  }
}

template <typename T>
asio::experimental::coro<T>
countedGenerator(asio::io_context &, asio::experimental::coro<T> &generator,
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
asio::experimental::coro<T> oddifier(asio::io_context &,
                                     asio::experimental::coro<T> &generator) {
  while (auto n = co_await generator) {
    co_yield (*n % 2 == 0) ? *n + 1 : *n;
  }
}

template <typename G>
asio::experimental::coro<void> linePrinter(asio::io_context &, G &generator) {
  while (auto n = co_await generator) {
    std::cout << *n << '\n';
  }
}

template <typename G>
asio::awaitable<void> launcher(asio::io_context &, G &generator) {
  co_await generator.async_resume(asio::use_awaitable);
}

int main() {
  std::cout << "Starting..." << std::endl;

  try {
    asio::io_context io_context{};

    auto rg = randomGenerator(io_context, 0, 99);
    auto o = oddifier(io_context, rg);
    auto cg = countedGenerator(io_context, o, 20);
    auto lp = linePrinter(io_context, cg);

    co_spawn(io_context, launcher(io_context, lp), asio::detached);

    io_context.run();

    std::cout << "Clean shutdown." << std::endl;

    return EXIT_SUCCESS;
  } catch (const std::exception &e) {
    std::cerr << "Main exception: " << e.what() << std::endl;
  }

  return EXIT_FAILURE;
}

// Once std::generator is widely available in C++23, this could be achieved
// without ASIO:
/*
#include <coroutine>
#include <experimental/generator>
#include <iostream>
#include <random>

std::experimental::generator<int> randomGenerator(int min, int max)
{
    std::random_device rd{};
    std::default_random_engine re{ rd() };
    std::uniform_int_distribution<int> uid{ min, max };

    while ( true )
    {
        co_yield uid(rd);
    }
}

template<typename G>
std::experimental::generator<int> countedGenerator(G& generator, int count)
{
    auto it = generator.begin();
    for ( auto i = 0 ; i < count ; ++i )
    {
        co_yield *it;
        ++it;
    }
}

template<typename G>
std::experimental::generator<int> oddifier(G& generator)
{
    for ( const auto & i : generator )
    {
        co_yield (i%2==0) ? i+1 : i;
    }
}

int main()
{
    auto rg = randomGenerator(0, 99);
    auto o = oddifier(rg);
    for ( const auto & i :
      countedGenerator<std::experimental::generator<int>>(o, 20) )
    {
        std::cout << i << '\n';
    }
    std::cout << "Done" << std::endl;
}
*/
