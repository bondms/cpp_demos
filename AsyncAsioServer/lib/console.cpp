// Copyright 2021 Mark Bond

#include "AsyncAsioServer/lib/console.h"

#include <iostream>

Console::Console(asio::io_context &io_context, TcpServer &server)
    : io_context_{io_context}, server_{server}, input_{io_context,
                                                       ::dup(STDIN_FILENO)} {
  async_wait_for_quit();
}

Console::~Console() { input_.close(); }

void Console::async_wait_for_quit() {
  std::cout << "Enter 'q' and press return to quit." << std::endl;
  input_buffer_.consume(input_buffer_.size());

  asio::async_read_until(
      input_, input_buffer_, '\n',
      [&](const asio::error_code &error, std::size_t bytes_transferred) {
        if (error) {
          std::cerr << "Error reading from standard input: " << error.value()
                    << std::endl;
          server_.shutdown();
          return;
        }

        if (2 != bytes_transferred) {
          async_wait_for_quit();
          return;
        }

        std::istream is(&input_buffer_);
        char ch{};
        is.get(ch);

        if ('q' != ch) {
          async_wait_for_quit();
          return;
        }

        std::cout << "Shutting down." << std::endl;
        server_.shutdown();
      });
}

// TODO(MarkBond):
// *Provide Windows implementation for reading from the console.
// * Fully understand reading from the console.
