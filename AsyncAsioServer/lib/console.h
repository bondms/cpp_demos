// Copyright 2021 Mark Bond

#pragma once

#include "AsyncAsioServer/lib/tcp_server.h"

class Console {
  asio::io_context &io_context_;
  TcpServer &server_;

  asio::posix::stream_descriptor input_;

  asio::streambuf input_buffer_{};

  void async_wait_for_quit();

public:
  Console(asio::io_context &io_context, TcpServer &server);

  Console(const Console &) = delete;
  Console &operator=(const Console &) = delete;

  ~Console();
};
