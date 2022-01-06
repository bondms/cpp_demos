// Copyright 2021 Mark Bond

#include "AsyncAsioServer/lib/tcp_connection.h"

#include <thread>

#include <gmock/gmock.h>

namespace {

class TcpConnectionTestFixture : public testing::Test {};

constexpr static asio::ip::port_type port{8013};

} // namespace

TEST_F(TcpConnectionTestFixture, Simple) {
  // std::thread server = []{
  //   asio::io_context io_context{};
  //   auto connection{TcpConnection::create(io_context)};

  //   io_context.run();
  // };

  /////

  // asio::io_context io_context{};

  // auto connection{TcpConnection::create(io_context)};

  // asio::ip::tcp::acceptor acceptor{
  //     io_context, asio::ip::tcp::endpoint{asio::ip::tcp::v4(), port}};

  // acceptor_.async_accept(connection->socket(),
  //                        [](const asio::error_code &error) {
  //                          xxx;
  //                        });

  // asio::ip::tcp::socket client_socket{io_context};
  // client_socket.open(asio::ip::tcp::v4());
  // client_socket.async_connect(
  //     asio::ip::tcp::endpoint{asio::ip::tcp::v4(), 8013},
  //     [](const asio::error_code & /*error*/) {});

  // std::vector<char> data(10, '\0');
  // client_socket.async_read_some(asio::buffer(data, data.size()),
  //                               [](const asio::error_code & /*error*/,
  //                                  std::size_t /*bytes_transferred*/) {});
}
