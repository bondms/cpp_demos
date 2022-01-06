// Copyright 2021 Mark Bond

#include "AsyncAsioServer/lib/tcp_server.h"

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

  std::string actual{};
  std::vector<char> buffer(8, '\0');

  std::function<void()> async_read_from_client{[&]() {
    client_socket.async_read_some(
        asio::buffer(buffer, buffer.size()),
        [&](const asio::error_code &error, std::size_t bytes_transferred) {
          if (error) {
            if (asio::error::misc_errors::eof != error.value()) {
              ADD_FAILURE()
                  << "Error (" << error.value() << "): " << error.message();
            }
            server.shutdown();
            return;
          }
          EXPECT_GT(bytes_transferred, 0);
          EXPECT_LE(bytes_transferred, buffer.size());
          std::copy(buffer.begin(),
                    buffer.begin() +
                        static_cast<std::vector<char>::difference_type>(
                            bytes_transferred),
                    std::back_inserter(actual));
          async_read_from_client();
        });
  }};

  async_read_from_client();
  io_context.run();

  EXPECT_EQ("9\n8\n7\n6\n5\n4\n3\n2\n1\n0\n", actual);
}

// TODO(MarkBond):
// * Make the server accept paramaters (which will pass through to the countdown
// timer) to configure start_from and interval.
// * For the simple test, use a smaller interval.
// * Introduce a slow test which uses a smaller start_from but a reasonable
// interval and checks the time is at least as long as expected.
// * Check for error in async_connect.
// * Test handling of multiple concucurrent clients.
// * Test handling when a client disconnects early.
