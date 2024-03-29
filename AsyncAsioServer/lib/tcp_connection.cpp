// Copyright 2021 Mark Bond
//
// Originally based on ASIO tutorial "An asynchronous TCP daytime server"
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "AsyncAsioServer/lib/tcp_connection.h"

#include <iostream>

void TcpConnection::handle_write(const asio::error_code &error,
                                 size_t bytes_transferred) {
  if (error) {
    std::cerr << "Aborting countdown on error " << error.value()
              << " sending: " << error.message() << std::endl;
    countdown_timer_.abort();
    return;
  }

  std::cout << "Sent bytes: " << bytes_transferred << std::endl;
}

TcpConnection::TcpConnection(PrivateConstruction, asio::io_context &io_context,
                             int start_from, std::chrono::milliseconds interval)
    : socket_{io_context}, countdown_timer_{io_context},
      start_from_{start_from}, interval_{interval} {}

TcpConnection::SharedPointer
TcpConnection::create(asio::io_context &io_context, int start_from,
                      std::chrono::milliseconds interval) {
  return std::make_shared<TcpConnection>(PrivateConstruction{}, io_context,
                                         start_from, interval);
}

asio::ip::tcp::socket &TcpConnection::socket() { return socket_; }

void TcpConnection::start() {
  std::cout << "Initiating countdown." << std::endl;
  countdown_timer_.initiate(
      start_from_, interval_,
      [this, shared_this = shared_from_this()](int value) {
        std::cout << "Countdown: " << value << std::endl;
        message_ = std::to_string(value) + "\n";
        asio::async_write(
            socket_, asio::buffer(message_),
            [shared_this = shared_from_this()](const asio::error_code &error,
                                               size_t bytes_transferred) {
              shared_this->handle_write(error, bytes_transferred);
            });
      });
}
