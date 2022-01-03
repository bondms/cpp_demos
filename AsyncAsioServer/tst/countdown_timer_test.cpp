// Copyright 2021 Mark Bond

#include "AsyncAsioServer/lib/countdown_timer.h"

#include <gmock/gmock.h>

#include <limits>

using std::chrono_literals::operator""h;
using std::chrono_literals::operator""ms;
using std::chrono_literals::operator""s;

namespace {

class CountdownTimerTestFixture : public testing::Test {};

} // namespace

TEST_F(CountdownTimerTestFixture, Simple) {
  asio::io_context io_context{};

  std::vector<int> counts{};

  CountdownTimer timer{io_context};
  timer.initiate(5, 1ms, [&](int value) { counts.push_back(value); });

  io_context.run();

  EXPECT_THAT(counts, testing::ElementsAre(4, 3, 2, 1, 0));
}

TEST_F(CountdownTimerTestFixture, StartFromZero) {
  asio::io_context io_context{};

  std::vector<int> counts{};

  CountdownTimer timer{io_context};
  timer.initiate(0, 1ms, [&](int value) { counts.push_back(value); });

  io_context.run();

  EXPECT_TRUE(counts.empty());
}

TEST_F(CountdownTimerTestFixture, StartFromNegative) {
  asio::io_context io_context{};

  std::vector<int> counts{};

  CountdownTimer timer{io_context};
  timer.initiate(-5, 1ms, [&](int value) { counts.push_back(value); });

  io_context.run();

  EXPECT_TRUE(counts.empty());
}

TEST_F(CountdownTimerTestFixture, StartFromMaxNegative) {
  asio::io_context io_context{};

  std::vector<int> counts{};

  CountdownTimer timer{io_context};
  timer.initiate(std::numeric_limits<int>::min(), 1ms,
                 [&](int value) { counts.push_back(value); });

  io_context.run();

  EXPECT_TRUE(counts.empty());
}

TEST_F(CountdownTimerTestFixture, abort_MidSequence) {
  asio::io_context io_context{};

  std::vector<int> counts{};

  CountdownTimer timer{io_context};
  timer.initiate(10, 1ms, [&](int value) {
    if (6 == value) {
      timer.abort();
      return;
    }
    counts.push_back(value);
  });

  io_context.run();

  EXPECT_THAT(counts, testing::ElementsAre(9, 8, 7));
}

TEST_F(CountdownTimerTestFixture, abort_Early) {
  asio::io_context io_context{};

  std::vector<int> counts{};

  CountdownTimer timer{io_context};
  timer.initiate(10, 1h, [&](int value) { counts.push_back(value); });

  asio::post([&] { timer.abort(); });

  io_context.run();

  EXPECT_TRUE(counts.empty());
}

TEST_F(CountdownTimerTestFixture, WaitsForInterval_Slow) {
  asio::io_context io_context{};

  auto previous{std::chrono::steady_clock::now()};

  CountdownTimer timer{io_context};
  timer.initiate(3, 1s, [&](int /*value*/) {
    const auto current{std::chrono::steady_clock::now()};
    EXPECT_GE(current, previous + 1s);
    previous = current;
  });

  io_context.run();
}
