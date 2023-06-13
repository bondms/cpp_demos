// Copyright 2023 Mark Bond
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <asio.hpp>

template <typename G>
asio::awaitable<void> launcher(asio::io_context &, G &generator) {
  co_await generator.async_resume(asio::use_awaitable);
}
