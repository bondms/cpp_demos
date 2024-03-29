// Copyright 2021 Mark Bond
//
// Originally based on ASIO tutorial "An asynchronous TCP daytime server"
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <memory>
#include <string>

#include "AsyncAsioServer/lib/countdown_timer.h"

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
  struct PrivateConstruction {};

  asio::ip::tcp::socket socket_;
  CountdownTimer countdown_timer_;
  const int start_from_;
  const std::chrono::milliseconds interval_;

  std::string message_{};

  void handle_write(const asio::error_code &error, size_t bytes_transferred);

public:
  TcpConnection(PrivateConstruction, asio::io_context &io_context,
                int start_from, std::chrono::milliseconds interval);

  using SharedPointer = std::shared_ptr<TcpConnection>;
  static SharedPointer create(asio::io_context &io_context, int start_from,
                              std::chrono::milliseconds interval);

  asio::ip::tcp::socket &socket();

  void start();
};
