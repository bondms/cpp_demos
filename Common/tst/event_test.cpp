// Copyright 2021 Mark Bond

#include "common/event.h"

#include <gmock/gmock.h>

#include <atomic>
#include <thread>

using std::chrono_literals::operator""s;

namespace {

class EventTestFixture :
    public testing::Test {
};

}  // namespace

TEST_F(EventTestFixture, signalAndWait_Quick) {
    Event e1{ Event::Mode::manualReset, Event::State::nonSignalled };
    Event e2{ Event::Mode::manualReset, Event::State::nonSignalled };
    int i{ 0 };

    std::thread t{[&]{
        EXPECT_EQ(0, i);
        ++i;
        e1.Signal();
        e2.Wait();
        EXPECT_EQ(2, i);
    }};

    e1.Wait();
    EXPECT_EQ(1, i);
    ++i;
    e2.Signal();

    t.join();
}

TEST_F(EventTestFixture, waitForAndManualReset_Quick) {
    Event e{ Event::Mode::manualReset, Event::State::nonSignalled };
    EXPECT_FALSE(e.WaitFor(0s));
    EXPECT_FALSE(e.WaitFor(0s));

    e.Signal();
    EXPECT_TRUE(e.WaitFor(0s));
    EXPECT_TRUE(e.WaitFor(0s));

    e.Reset();
    EXPECT_FALSE(e.WaitFor(0s));
    EXPECT_FALSE(e.WaitFor(0s));
}

TEST_F(EventTestFixture, autoResetAndSignalledInitialState_Quick) {
    Event e{ Event::Mode::autoReset, Event::State::signalled };
    EXPECT_TRUE(e.WaitFor(0s));
    EXPECT_FALSE(e.WaitFor(0s));

    e.Signal();
    EXPECT_TRUE(e.WaitFor(0s));
    EXPECT_FALSE(e.WaitFor(0s));
}

TEST_F(EventTestFixture, autoReset_SignalsOnlyOnce_Quick) {
    Event e{ Event::Mode::autoReset, Event::State::nonSignalled };
    std::atomic_int i{ 0 };

    std::thread t{[&]{
        for ( auto j = 0 ; j < 2 ; ++j ) {
            e.Wait();
            ++i;
        }
    }};

    EXPECT_EQ(0, i);

    e.Signal();
    while ( i < 1 ) {
        // Spin.
    }
    EXPECT_EQ(1, i);

    e.Signal();
    t.join();
    EXPECT_EQ(2, i);
}

TEST_F(EventTestFixture, autoReset_SignalsOnlyOnce_Slow) {
    Event e{ Event::Mode::autoReset, Event::State::nonSignalled };
    std::atomic_int i{ 0 };

    std::thread t{[&]{
        for ( auto j = 0 ; j < 2 ; ++j ) {
            e.Wait();
            ++i;
        }
    }};

    EXPECT_EQ(0, i);

    e.Signal();
    std::this_thread::sleep_for(1s);
    EXPECT_EQ(1, i);

    e.Signal();
    t.join();
    EXPECT_EQ(2, i);
}

TEST_F(EventTestFixture, autoReset_SignalsOnlyOne_Quick) {
    Event e{ Event::Mode::autoReset, Event::State::nonSignalled };
    std::atomic_int i{ 0 };

    std::thread t1{[&]{
        e.Wait();
        ++i;
    }};

    std::thread t2{[&]{
        e.Wait();
        ++i;
    }};

    EXPECT_EQ(0, i);

    e.Signal();
    while ( i < 1 ) {
        // Spin.
    }
    EXPECT_EQ(1, i);

    e.Signal();
    t1.join();
    t2.join();
    EXPECT_EQ(2, i);
}

TEST_F(EventTestFixture, autoReset_SignalsOnlyOne_Slow) {
    Event e{ Event::Mode::autoReset, Event::State::nonSignalled };
    std::atomic_int i{ 0 };

    std::thread t1{[&]{
        e.Wait();
        ++i;
    }};

    std::thread t2{[&]{
        e.Wait();
        ++i;
    }};

    std::this_thread::sleep_for(1s);
    EXPECT_EQ(0, i);

    e.Signal();
    std::this_thread::sleep_for(1s);
    EXPECT_EQ(1, i);

    e.Signal();
    t1.join();
    t2.join();
    EXPECT_EQ(2, i);
}

TEST_F(EventTestFixture, manualReset_SignalsAll_Quick) {
    Event e{ Event::Mode::manualReset, Event::State::nonSignalled };
    std::atomic_int i{ 0 };

    std::thread t1{[&]{
        e.Wait();
        ++i;
    }};

    std::thread t2{[&]{
        e.Wait();
        ++i;
    }};

    EXPECT_EQ(0, i);

    e.Signal();
    t1.join();
    t2.join();
    EXPECT_EQ(2, i);
}

TEST_F(EventTestFixture, manualReset_SignalsAll_Slow) {
    Event e{ Event::Mode::manualReset, Event::State::nonSignalled };
    std::atomic_int i{ 0 };

    std::thread t1{[&]{
        e.Wait();
        ++i;
    }};

    std::thread t2{[&]{
        e.Wait();
        ++i;
    }};

    std::this_thread::sleep_for(1s);
    EXPECT_EQ(0, i);

    e.Signal();
    t1.join();
    t2.join();
    EXPECT_EQ(2, i);
}

TEST_F(EventTestFixture, WaitFor_Slow) {
    Event e{ Event::Mode::manualReset, Event::State::nonSignalled };

    const auto startTime{ std::chrono::steady_clock::now() };

    std::thread t{[&]{
        std::this_thread::sleep_until(startTime + 1s);
        e.Signal();
    }};

    EXPECT_FALSE(e.WaitFor(0s));
    EXPECT_LT(std::chrono::steady_clock::now(), startTime + 1s);
    EXPECT_TRUE(e.WaitFor(10s));

    const auto endTime{ std::chrono::steady_clock::now() };

    EXPECT_LT(endTime, startTime + 2s);
    EXPECT_GE(endTime, startTime + 1s);

    t.join();
}

TEST_F(EventTestFixture, WaitUntil_Quick) {
    const auto startTime{ std::chrono::steady_clock::now() };

    Event e{ Event::Mode::manualReset, Event::State::nonSignalled };
    EXPECT_FALSE(e.WaitUntil(startTime));
    e.Signal();
    EXPECT_TRUE(e.WaitUntil(startTime));
}

TEST_F(EventTestFixture, WaitUntil_Slow) {
    Event e{ Event::Mode::manualReset, Event::State::nonSignalled };

    const auto startTime{ std::chrono::steady_clock::now() };

    std::thread t{[&]{
        std::this_thread::sleep_until(startTime + 1s);
        e.Signal();
    }};

    EXPECT_FALSE(e.WaitUntil(startTime));
    EXPECT_LT(std::chrono::steady_clock::now(), startTime + 1s);
    EXPECT_TRUE(e.WaitUntil(startTime + 10s));

    const auto endTime{ std::chrono::steady_clock::now() };

    EXPECT_LT(endTime, startTime + 2s);
    EXPECT_GE(endTime, startTime + 1s);

    t.join();
}
