// Copyright 2021 Mark Bond
//
// Originally based on ASIO tutorial "An asynchronous TCP daytime server"
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "AsyncAsioServer/lib/tcp_connection.h"

class TcpServer {
  asio::io_context &io_context_;
  asio::ip::tcp::acceptor acceptor_;
  int start_from_;
  std::chrono::milliseconds interval_;

  void start_accept();

  void handle_accept(TcpConnection::SharedPointer new_connection,
                     const asio::error_code &error);

public:
  constexpr static asio::ip::port_type port{8013};

  explicit TcpServer(asio::io_context &io_context, int start_from,
                     std::chrono::milliseconds interval);

  void shutdown();
};
