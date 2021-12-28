// Copyright 2021 Mark Bond
//
// Originally based on ASIO tutorial "An asynchronous TCP daytime server"
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>

#include <asio.hpp>

using asio::ip::tcp;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
  struct PrivateConstruction {};

  tcp::socket socket_;
  std::string message_{};

  void handle_write(const asio::error_code & /*error*/,
                    size_t /*bytes_transferred*/) {}

public:
  TcpConnection(PrivateConstruction, asio::io_context &io_context)
      : socket_{io_context} {}

  using SharedPointer = std::shared_ptr<TcpConnection>;

  static SharedPointer create(asio::io_context &io_context) {
    return std::make_shared<TcpConnection>(PrivateConstruction{}, io_context);
  }

  tcp::socket &socket() { return socket_; }

  void start() {
    const time_t now{std::time(0)};
    message_ = std::ctime(&now);

    std::cout << "Connection accepted; sending message: " << message_
              << std::flush;

    asio::async_write(
        socket_, asio::buffer(message_),
        [shared_this = shared_from_this()](const asio::error_code &error,
                                           size_t bytes_transferred) {
          shared_this->handle_write(error, bytes_transferred);
        });
  }
};

class TcpServer {
  asio::io_context &io_context_;
  tcp::acceptor acceptor_;

  void start_accept() {
    TcpConnection::SharedPointer new_connection{
        TcpConnection::create(io_context_)};

    acceptor_.async_accept(
        new_connection->socket(),
        [this, new_connection](const asio::error_code &error) {
          this->handle_accept(new_connection, error);
        });
  }

  void handle_accept(TcpConnection::SharedPointer new_connection,
                     const asio::error_code &error) {
    if (!error) {
      new_connection->start();
    }

    start_accept();
  }

public:
  explicit TcpServer(asio::io_context &io_context)
      : io_context_{io_context}, acceptor_{io_context,
                                           tcp::endpoint{tcp::v4(), 8013}} {
    start_accept();
  }
};

int main() {
  std::cout << "Starting..." << std::endl;

  try {
    asio::io_context io_context{};
    TcpServer server{io_context};
    io_context.run();

    std::cout << "Shutting down..." << std::endl;
    return EXIT_SUCCESS;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return EXIT_FAILURE;
}
