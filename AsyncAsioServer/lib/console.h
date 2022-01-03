// Copyright 2021 Mark Bond

#pragma once

#include "AsyncAsioServer/lib/tcp_server.h"

#include <asio.hpp>

class Console {
  asio::io_context &io_context_;
  TcpServer &server_;

  asio::posix::stream_descriptor input_;

  char data_{};

public:
  Console(asio::io_context &io_context, TcpServer &server);

  void async_wait_for_quit();
};
