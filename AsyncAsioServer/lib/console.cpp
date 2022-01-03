// Copyright 2021 Mark Bond

#include "AsyncAsioServer/lib/console.h"

#include <iostream>

Console::Console(asio::io_context &io_context, TcpServer &server)
    : io_context_{io_context}, server_{server}, input_{io_context,
                                                       ::dup(STDIN_FILENO)} {
  std::cout << "Press any key to quit." << std::endl;

  input_.read_some(
      asio::buffer(&data_, sizeof(data_)),
      [&](const asio::error_code &error, std::size_t /*bytes_transferred*/) {
        if (error) {
          std::cerr << "Error reading from standard input: " << error.value()
                    << std::endl;
        } else {
          std::cout << "Shutting down." << std::endl;
        }
        server_.shutdown();
        input_.close();
      });
}
