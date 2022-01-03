// Copyright 2021 Mark Bond

#include "AsyncAsioServer/lib/countdown_timer.h"

#include <gmock/gmock.h>

using std::chrono_literals::operator""h;
using std::chrono_literals::operator""ms;

namespace {

class CountdownTimerTestFixture : public testing::Test {};

} // namespace

TEST_F(CountdownTimerTestFixture, simple) {
  asio::io_context io_context{};

  std::vector<int> counts{};

  CountdownTimer timer{io_context, 1ms};
  timer.initiate([&](int value) { counts.push_back(value); });

  io_context.run();

  EXPECT_THAT(counts, testing::ElementsAre(9, 8, 7, 6, 5, 4, 3, 2, 1, 0));
}

TEST_F(CountdownTimerTestFixture, abort_mid_sequence) {
  asio::io_context io_context{};

  std::vector<int> counts{};

  CountdownTimer timer{io_context, 1ms};
  timer.initiate([&](int value) {
    if (5 == value) {
      timer.abort();
      return;
    }
    counts.push_back(value);
  });

  io_context.run();

  EXPECT_THAT(counts, testing::ElementsAre(9, 8, 7, 6));
}

TEST_F(CountdownTimerTestFixture, abort_early) {
  asio::io_context io_context{};

  std::vector<int> counts{};

  CountdownTimer timer{io_context, 1h};
  timer.initiate([&](int value) { counts.push_back(value); });

  asio::post([&] { timer.abort(); });

  io_context.run();

  EXPECT_TRUE(counts.empty());
}
