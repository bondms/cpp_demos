// Copyright 2021 Mark Bond

#pragma once

#include <asio.hpp>

class Console {
  asio::io_context &io_context_;
  TcpServer &server_;

public:
  Console(asio::io_context &io_context, TcpServer &server);
};
