// Copyright 2021 Mark Bond

#include "AsyncAsioServer/lib/console.h"

#include <iostream>

Console::Console(asio::io_context &io_context, TcpServer &server)
    : io_context_{io_context}, server_{server}, input_{io_context,
                                                       ::dup(STDIN_FILENO)} {
  async_wait_for_quit();
}

void Console::async_wait_for_quit() {
  std::cout << "Enter 'q' and press return to quit." << std::endl;

  input_.async_read_some(
      asio::buffer(&data_, sizeof(data_)),
      [&](const asio::error_code &error, std::size_t /*bytes_transferred*/) {
        if (error) {
          std::cerr << "Error reading from standard input: " << error.value()
                    << std::endl;
        } else if ('q' == data_) {
          std::cout << "Shutting down." << std::endl;
        } else {
          async_wait_for_quit();
          return;
        }
        server_.shutdown();
        input_.close();
      });
}
