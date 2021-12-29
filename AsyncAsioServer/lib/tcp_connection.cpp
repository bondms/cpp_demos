// Copyright 2021 Mark Bond
//
// Originally based on ASIO tutorial "An asynchronous TCP daytime server"
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "AsyncAsioServer/lib/tcp_connection.h"

#include <chrono>
#include <ctime>
#include <iostream>

using namespace std::chrono_literals;

void TcpConnection::handle_write(const asio::error_code &error,
                                 size_t bytes_transferred) {
  if (!error) {
    std::cout << "Sent bytes: " << bytes_transferred << std::endl;

    timer_ = std::make_unique<asio::steady_timer>(io_context_, 10s);
    timer_->async_wait([shared_this = shared_from_this()](
                           const asio::error_code &timer_error) {
      if (!timer_error) {
        std::cout << "Timer expired; closing connection." << std::endl;
        return;
      }

      std::cerr << "Error waiting for timer, value: " << timer_error.value()
                << std::endl;
    });

    return;
  }

  std::cerr << "Error sending, value: " << error.value() << std::endl;
}

TcpConnection::TcpConnection(PrivateConstruction, asio::io_context &io_context)
    : io_context_{io_context}, socket_{io_context} {}

TcpConnection::SharedPointer
TcpConnection::create(asio::io_context &io_context) {
  return std::make_shared<TcpConnection>(PrivateConstruction{}, io_context);
}

asio::ip::tcp::socket &TcpConnection::socket() { return socket_; }

void TcpConnection::start() {
  const time_t now{std::time(0)};
  message_ = std::ctime(&now);

  std::cout << "Sending message: " << message_ << std::flush;

  asio::async_write(
      socket_, asio::buffer(message_),
      [shared_this = shared_from_this()](const asio::error_code &error,
                                         size_t bytes_transferred) {
        shared_this->handle_write(error, bytes_transferred);
      });
}
