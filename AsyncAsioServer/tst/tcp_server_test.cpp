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

TEST_F(TcpServerTestFixture, SequentialConnections) {
  asio::io_context io_context{};

  TcpServer server{io_context, 3, 1ms};

  asio::ip::tcp::socket client_socket{io_context};

  auto connect = [&]() {
    client_socket.async_connect(
        asio::ip::tcp::endpoint{asio::ip::address::from_string("127.0.0.1"),
                                TcpServer::port},
        [](const asio::error_code &error) {
          if (error) {
            ADD_FAILURE() << "Error (" << error.value()
                          << "): " << error.message();
          }
        });
  };
  connect();

  constexpr auto &expected{"2\n1\n0\n"};
  std::array<char, std::size(expected) + 1> buffer{};
  auto it{buffer.begin()};
  int attempt{0};

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
            if (++attempt < 2) {
              connect();
              async_read_from_client();
            } else {
              server.shutdown();
            }
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

  auto remaining_clients{client_sockets.size()};

  std::function<void(std::size_t)> async_read_from_client{[&](std::size_t i) {
    const auto remaining_size{static_cast<std::size_t>(
        std::distance(buffers[i].it, buffers[i].data.end()))};
    client_sockets[i].async_read_some(
        asio::buffer(buffers[i].it, remaining_size),
        [&, i, remaining_size](const asio::error_code &error,
                               std::size_t bytes_transferred) {
          if (error) {
            if (asio::error::misc_errors::eof != error.value()) {
              ADD_FAILURE()
                  << "Error (" << error.value() << "): " << error.message();
            }
            if (0 == --remaining_clients) {
              server.shutdown();
            }
            return;
          }
          EXPECT_GT(bytes_transferred, 0);
          EXPECT_LE(bytes_transferred, remaining_size);
          buffers[i].it += bytes_transferred;
          async_read_from_client(i);
        });
  }};

  for (std::size_t i = 0; i < client_sockets.size(); ++i) {
    async_read_from_client(i);
  }

  io_context.run();

  for (const auto &buffer : buffers) {
    EXPECT_EQ(expected,
              std::string(buffer.data.cbegin(),
                          static_cast<decltype(buffer.data)::const_iterator>(
                              buffer.it)));
  }
}

TEST_F(TcpServerTestFixture, AbortAfterFirst) {
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

  constexpr auto &expected{"2\n"};
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
            ADD_FAILURE() << "Error (" << error.value()
                          << "): " << error.message();
            server.shutdown();
            return;
          }
          EXPECT_GT(bytes_transferred, 0);
          EXPECT_LE(bytes_transferred, remaining_size);
          it += bytes_transferred;
          server.shutdown();
        });
  }};

  async_read_from_client();
  io_context.run();

  EXPECT_EQ(expected, std::string(buffer.begin(), it));
}

TEST_F(TcpServerTestFixture, ReconnectAfterClientDisconnect) {
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

  constexpr auto &expected{"2\n"};
  std::array<char, std::size(expected) + 1> buffer{};
  auto it{buffer.begin()};
  int attempt{0};

  std::function<void()> async_read_from_client{[&]() {
    const auto remaining_size{
        static_cast<std::size_t>(std::distance(it, buffer.end()))};
    client_socket.async_read_some(
        asio::buffer(it, remaining_size),
        [&, remaining_size](const asio::error_code &error,
                            std::size_t bytes_transferred) {
          if (error) {
            ADD_FAILURE() << "Error (" << error.value()
                          << "): " << error.message();
            if (2 == ++attempt) {
              server.shutdown();
            }
            return;
          }
          EXPECT_GT(bytes_transferred, 0);
          EXPECT_LE(bytes_transferred, remaining_size);
          it += bytes_transferred;
          server.shutdown();
        });
  }};

  async_read_from_client();
  io_context.run();

  EXPECT_EQ(expected, std::string(buffer.begin(), it));
}
