// Copyright 2021 Mark Bond

#include "AsyncAsioServer/lib/console.h"

Console::Console(asio::io_context &io_context, TcpServer &server)
    : io_context_{io_context}, server_{server} {}
