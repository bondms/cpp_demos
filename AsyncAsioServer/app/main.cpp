// Copyright 2021 Mark Bond
//
// Originally based on ASIO tutorial "An asynchronous TCP daytime server"
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>
#include <iostream>
#include <thread>

#include "AsyncAsioServer/lib/tcp_server.h"

int main() {
  std::cout << "Starting..." << std::endl;
  std::cout << "Main thread ID: " << std::this_thread::get_id() << std::endl;

  try {
    asio::io_context io_context{};
    TcpServer server{io_context};

    std::thread worker{[&]() {
      try {
        std::cout << "Worker thread ID: " << std::this_thread::get_id()
                  << std::endl;
        io_context.run();
      } catch (const std::exception &e) {
        std::cerr << "Worker exception: " << e.what() << std::endl;
      }
    }};

    char ch{};
    while ('q' != ch) {
      std::cout << "Enter 'q' and press return to quit." << std::endl;
      std::cin >> ch;
    }

    std::cout << "Signalling server to shut down." << std::endl;
    server.shutdown();

    std::cout << "Waiting to server to shut down." << std::endl;
    worker.join();

    std::cout << "Clean shutdown." << std::endl;

    return EXIT_SUCCESS;
  } catch (const std::exception &e) {
    std::cerr << "Main exception: " << e.what() << std::endl;
  }

  return EXIT_FAILURE;
}
