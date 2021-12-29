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
#include <mutex>
#include <string>
#include <thread>

#include <asio.hpp>

using asio::ip::tcp;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
  struct PrivateConstruction {};

  tcp::socket socket_;
  std::string message_{};

  void handle_write(const asio::error_code &error, size_t bytes_transferred) {
    if (!error) {
      std::cout << "Sent bytes: " << bytes_transferred << std::endl;
      return;
    }

    std::cerr << "Error sending, value: " << error.value() << std::endl;
  }

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

    std::cout << "Sending message: " << message_ << std::flush;

    asio::async_write(
        socket_, asio::buffer(message_),
        [shared_this = shared_from_this()](const asio::error_code &error,
                                           size_t bytes_transferred) {
          shared_this->handle_write(error, bytes_transferred);
        });
  }
};

class TcpServer {
  constexpr static asio::ip::port_type port_{8013};

  asio::io_context &io_context_;
  tcp::acceptor acceptor_;
  std::mutex mutex_{};

  void start_accept() {
    TcpConnection::SharedPointer new_connection{
        TcpConnection::create(io_context_)};

    std::lock_guard<std::mutex> lock{mutex_};
    acceptor_.async_accept(
        new_connection->socket(),
        [this, new_connection](const asio::error_code &error) {
          this->handle_accept(new_connection, error);
        });
  }

  void handle_accept(TcpConnection::SharedPointer new_connection,
                     const asio::error_code &error) {
    if (asio::error::operation_aborted == error.value()) {
      std::cout << "Aborting accept for new connection." << std::endl;
      // Return without starting a fresh accept to allow clean shutdown.
      return;
    }

    if (!error) {
      std::cout << "Connection accepted." << std::endl;
      new_connection->start();
    } else {
      std::cerr << "Error accepting connection, value: " << error.value()
                << std::endl;
    }

    std::cout << "Accept next connection." << std::endl;
    start_accept();
  }

public:
  explicit TcpServer(asio::io_context &io_context)
      : io_context_{io_context}, acceptor_{io_context,
                                           tcp::endpoint{tcp::v4(), port_}} {
    std::cout << "Listening on port: " << port_ << std::endl;

    std::cout << "Accept first connection." << std::endl;
    start_accept();
  }

  void shutdown() {
    std::lock_guard<std::mutex> lock{mutex_};
    acceptor_.close();
  }
};

int main() {
  std::cout << "Starting..." << std::endl;

  try {
    asio::io_context io_context{};
    TcpServer server{io_context};

    std::thread worker{[&]() { io_context.run(); }};

    std::cout << "Enter 'q' and press return to quit." << std::endl;
    char ch{};
    while ('q' != ch) {
      std::cin >> ch;
    }

    std::cout << "Signalling server to shut down." << std::endl;
    server.shutdown();

    std::cout << "Waiting to server to shut down." << std::endl;
    worker.join();

    std::cout << "Clean shutdown." << std::endl;

    return EXIT_SUCCESS;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return EXIT_FAILURE;
}
