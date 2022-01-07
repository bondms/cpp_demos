// Copyright 2021 Mark Bond

#include "AsyncAsioServer/lib/tcp_server.h"

#include <gmock/gmock.h>

using std::chrono_literals::operator""ms;

namespace {

class TcpServerTestFixture : public testing::Test {};

} // namespace

TEST_F(TcpServerTestFixture, Simple) {
  asio::io_context io_context{};

  TcpServer server{io_context, 3, 1ms};

  asio::ip::tcp::socket client_socket{io_context};
  client_socket.async_connect(
      asio::ip::tcp::endpoint{asio::ip::address::from_string("127.0.0.1"),
                              TcpServer::port},
      [](const asio::error_code &error) {
        if (error) {
          ADD_FAILURE() << "Error (" << error.value()
                        << "): " << error.message();
        }
      });

  constexpr auto &expected{"2\n1\n0\n"};
  std::array<char, std::size(expected) + 1> buffer{};
  auto it{buffer.begin()};

  std::function<void()> async_read_from_client{[&]() {
    const auto remaining_size{
        static_cast<std::size_t>(std::distance(it, buffer.end()))};
    client_socket.async_read_some(
        asio::buffer(it, remaining_size),
        [&, remaining_size](const asio::error_code &error,
                            std::size_t bytes_transferred) {
          if (error) {
            if (asio::error::misc_errors::eof != error.value()) {
              ADD_FAILURE()
                  << "Error (" << error.value() << "): " << error.message();
            }
            server.shutdown();
            return;
          }
          EXPECT_GT(bytes_transferred, 0);
          EXPECT_LE(bytes_transferred, remaining_size);
          it += bytes_transferred;
          async_read_from_client();
        });
  }};

  async_read_from_client();
  io_context.run();

  EXPECT_EQ(expected, std::string(buffer.begin(), it));
}

TEST_F(TcpServerTestFixture, MultipleClients) {
  asio::io_context io_context{};

  TcpServer server{io_context, 3, 1ms};

  std::array client_sockets{asio::ip::tcp::socket{io_context},
                            asio::ip::tcp::socket{io_context},
                            asio::ip::tcp::socket{io_context}};

  for (auto &client_socket : client_sockets) {
    client_socket.async_connect(
        asio::ip::tcp::endpoint{asio::ip::address::from_string("127.0.0.1"),
                                TcpServer::port},
        [](const asio::error_code &error) {
          if (error) {
            ADD_FAILURE() << "Error (" << error.value()
                          << "): " << error.message();
          }
        });
  }

  constexpr auto &expected{"2\n1\n0\n"};

  struct Buffer {
    std::array<char, std::size(expected) + 1> data{};
    decltype(data)::iterator it{data.begin()};
  };
  std::array<Buffer, client_sockets.size()> buffers{};

  auto remaining{client_sockets.size()};

  for (std::size_t i = 0; i < client_sockets.size(); ++i) {
    std::function<void()> async_read_from_client{[&]() {
      const auto remaining_size{static_cast<std::size_t>(
          std::distance(buffers[i].it, buffers[i].data.end()))};
      client_sockets[i].async_read_some(
          asio::buffer(buffers[i].it, remaining_size),
          [&, remaining_size](const asio::error_code &error,
                              std::size_t bytes_transferred) {
            if (error) {
              if (asio::error::misc_errors::eof != error.value()) {
                ADD_FAILURE()
                    << "Error (" << error.value() << "): " << error.message();
              }
              if (0 == --remaining) {
                server.shutdown();
              }
              return;
            }
            EXPECT_GT(bytes_transferred, 0);
            EXPECT_LE(bytes_transferred, remaining_size);
            buffers[i].it += bytes_transferred;
            async_read_from_client();
          });
    }};

    async_read_from_client();
  }

  io_context.run();

  for (const auto &buffer : buffers) {
    EXPECT_EQ(expected,
              std::string(buffer.data.cbegin(),
                          static_cast<decltype(buffer.data)::const_iterator>(
                              buffer.it)));
  }
}

// TODO(MarkBond):
// * Introduce a slow test which uses a smaller start_from but a reasonable
// interval and checks the time is at least as long as expected.
// * Test handling when a client disconnects early.
// * Test with a large start_from. Generate the expected output with a helper
// function.
