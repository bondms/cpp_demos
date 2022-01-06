// Copyright 2021 Mark Bond

#include "AsyncAsioServer/lib/tcp_connection.h"

#include <gmock/gmock.h>

namespace {

class TcpConnectionTestFixture : public testing::Test {};

} // namespace

TEST_F(TcpConnectionTestFixture, Simple) {
  asio::io_context io_context{};

  auto connection{TcpConnection::create(io_context)};
  connection->socket().open(asio::ip::tcp::v4());
  connection->socket().bind(asio::ip::tcp::endpoint{asio::ip::tcp::v4(), 8013});

  asio::ip::tcp::socket client_socket{io_context};
  // client_socket.connect(asio::ip::tcp::endpoint{asio::ip::tcp::v4(), 8013});

  std::vector<char> data(10, '\0');
  client_socket.async_read_some(
      asio::buffer(data, data.size()),
      [](const asio::error_code &/*error*/, std::size_t /*bytes_transferred*/) {  });
}
