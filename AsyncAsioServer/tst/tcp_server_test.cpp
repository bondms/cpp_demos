// Copyright 2021 Mark Bond

#include "AsyncAsioServer/lib/tcp_server.h"

#include <vector>

#include <gmock/gmock.h>

namespace {

class TcpServerTestFixture : public testing::Test {};

} // namespace

TEST_F(TcpServerTestFixture, Simple) {
  asio::io_context io_context{};

  TcpServer server{io_context};

  asio::ip::tcp::socket client_socket{io_context};
  client_socket.open(asio::ip::tcp::v4());
  client_socket.async_connect(
      asio::ip::tcp::endpoint{asio::ip::tcp::v4(), TcpServer::port},
      [](const asio::error_code & /*error*/) {});

  std::vector<char> data(1024, '\0');
  client_socket.async_read_some(
      asio::buffer(data, 1024),
      [&](const asio::error_code &error, std::size_t bytes_transferred) {
        if (error) {
          ADD_FAILURE() << "Error: " << error.value();
          server.shutdown();
          return;
        }
        EXPECT_THAT(data, testing::ElementsAre('a', 'b', 'c'));
        EXPECT_GT(bytes_transferred, 0);
        server.shutdown();
      });

  io_context.run();
}
