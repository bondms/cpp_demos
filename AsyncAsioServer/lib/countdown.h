// Copyright 2021 Mark Bond

#pragma once

#include "AsyncAsioServer/lib/countdown_client_interface.h"

#include <memory>

#include <asio.hpp>

class Countdown {
  CountdownClientInterface & countdown_client_;

  std::unique_ptr<asio::steady_timer> timer_{};

public:
  explicit Countdown(CountdownClientInterface & countdown_client);

  void initiate(int start_from);
};
