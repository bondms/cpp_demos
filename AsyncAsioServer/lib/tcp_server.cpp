// Copyright 2021 Mark Bond
//
// Originally based on ASIO tutorial "An asynchronous TCP daytime server"
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "AsyncAsioServer/lib/tcp_server.h"

#include <iostream>

void TcpServer::start_accept() {
  TcpConnection::SharedPointer new_connection{
      TcpConnection::create(io_context_, start_from_, interval_)};

  acceptor_.async_accept(new_connection->socket(),
                         [this, new_connection](const asio::error_code &error) {
                           this->handle_accept(new_connection, error);
                         });
}

void TcpServer::handle_accept(TcpConnection::SharedPointer new_connection,
                              const asio::error_code &error) {
  if (asio::error::operation_aborted == error.value()) {
    std::cout << "Aborting accept for new connection." << std::endl;
    return;
  }

  if (!error) {
    std::cout << "Connection accepted." << std::endl;
    new_connection->start();
  } else {
    std::cerr << "Error " << error.value()
              << " accepting connection: " << error.message() << std::endl;
  }

  std::cout << "Accept next connection." << std::endl;
  start_accept();
}

TcpServer::TcpServer(asio::io_context &io_context, int start_from,
                     std::chrono::milliseconds interval)
    : io_context_{io_context}, acceptor_{io_context,
                                         asio::ip::tcp::endpoint{
                                             asio::ip::tcp::v4(), port}},
      start_from_{start_from}, interval_{interval} {
  std::cout << "Listening on port: " << port << std::endl;

  std::cout << "Accept first connection." << std::endl;
  start_accept();
}

void TcpServer::shutdown() { acceptor_.close(); }
