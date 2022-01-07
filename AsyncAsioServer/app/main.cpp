// Copyright 2021 Mark Bond
//
// Originally based on ASIO tutorial "An asynchronous TCP daytime server"
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>
#include <iostream>

#include "AsyncAsioServer/lib/console.h"
#include "AsyncAsioServer/lib/tcp_server.h"

using std::chrono_literals::operator""s;

int main() {
  std::cout << "Starting..." << std::endl;
  std::cout << "Main thread ID: " << std::this_thread::get_id() << std::endl;

  try {
    asio::io_context io_context{};
    TcpServer server{io_context, 10, 1s};
    Console console{io_context, server};

    io_context.run();

    std::cout << "Clean shutdown." << std::endl;

    return EXIT_SUCCESS;
  } catch (const std::exception &e) {
    std::cerr << "Main exception: " << e.what() << std::endl;
  }

  return EXIT_FAILURE;
}
