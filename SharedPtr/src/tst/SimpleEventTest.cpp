#include "../app/SimpleEvent.h"

#include <gtest/gtest.h>

#include <atomic>
#include <thread>

using namespace std::chrono_literals;

namespace
{
    class SimpleEventTestFixture :
        public testing::Test
    {
    };

} // namespace

TEST_F(SimpleEventTestFixture, SimpleSignalAndWait_Quick)
{
    SimpleEvent e1{};
    SimpleEvent e2{};
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

TEST_F(SimpleEventTestFixture, Signal_SignalsAll_Quick)
{
    SimpleEvent e{};
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
    EXPECT_LE(i, 2);

    t1.join();
    t2.join();

    EXPECT_EQ(2, i);
}

TEST_F(SimpleEventTestFixture, Signal_SignalsAll_Slow)
{
    SimpleEvent e{};
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
    EXPECT_EQ(i, 2);

    t1.join();
    t2.join();

    EXPECT_EQ(2, i);
}

TEST_F(SimpleEventTestFixture, WaitFor_Quick)
{
    SimpleEvent e{};
    EXPECT_FALSE(e.WaitFor(0s));
    e.Signal();
    EXPECT_TRUE(e.WaitFor(0s));
}

TEST_F(SimpleEventTestFixture, WaitFor_Slow)
{
    SimpleEvent e{};

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

TEST_F(SimpleEventTestFixture, WaitUntil_Quick)
{
    const auto startTime{ std::chrono::steady_clock::now() };

    SimpleEvent e{};
    EXPECT_FALSE(e.WaitUntil(startTime));
    e.Signal();
    EXPECT_TRUE(e.WaitUntil(startTime));
}

TEST_F(SimpleEventTestFixture, WaitUntil_Slow)
{
    SimpleEvent e{};

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
