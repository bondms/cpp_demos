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
  constexpr static asio::ip::port_type port_{8013};

  asio::io_context &io_context_;
  asio::ip::tcp::acceptor acceptor_;

  void start_accept();

  void handle_accept(TcpConnection::SharedPointer new_connection,
                     const asio::error_code &error);

public:
  explicit TcpServer(asio::io_context &io_context);

  void shutdown();
};
