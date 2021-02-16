#include "lib/flush_controller.h"

#include "gtest_helper.h"

using namespace std::chrono_literals;

namespace
{
    class MockClock
    {
    public:
        using time_point = std::chrono::steady_clock::time_point;
        using duration = std::chrono::steady_clock::duration;

    private:
        static time_point now_;

    public:
        static time_point now()
        {
            return now_;
        }

        static void advance_time(MockClock::duration duration)
        {
            now_ += duration;
        }
    };

    MockClock::time_point MockClock::now_{};

    class FlushControllerTestFixture :
        public testing::Test
    {
    };
}

TEST_F(FlushControllerTestFixture, SteadyClock_ImmediatelyDue)
{
    FlushController<std::chrono::steady_clock> flush_controller{0s};
    EXPECT_TRUE(flush_controller.is_due());
    EXPECT_TRUE(flush_controller.is_due());
}

TEST_F(FlushControllerTestFixture, SteadyClock_NotDue)
{
    FlushController<std::chrono::steady_clock> flush_controller{1h};
    EXPECT_FALSE(flush_controller.is_due());
    EXPECT_FALSE(flush_controller.is_due());
}

TEST_F(FlushControllerTestFixture, MockClock_Simple)
{
    FlushController<MockClock> flush_controller{10s};
    EXPECT_FALSE(flush_controller.is_due());
    MockClock::advance_time(1s);
    EXPECT_FALSE(flush_controller.is_due());
    MockClock::advance_time(10s);
    EXPECT_TRUE(flush_controller.is_due());
    MockClock::advance_time(1s);
    EXPECT_FALSE(flush_controller.is_due());
    MockClock::advance_time(10s);
    EXPECT_TRUE(flush_controller.is_due());
}
