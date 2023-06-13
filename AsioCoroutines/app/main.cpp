// Copyright 2023 Mark Bond
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <chrono>
#include <cstdlib>
#include <iostream>

#include <asio.hpp>

#include "AsioCoroutines/lib/counted_generator.h"
#include "AsioCoroutines/lib/oddifier.h"
#include "AsioCoroutines/lib/random_generator.h"
#include "AsioCoroutines/lib/throttled_line_printer.h"

using std::chrono_literals::operator""s;

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
