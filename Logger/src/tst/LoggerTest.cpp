#include "../app/Logger.h"

#include <gtest/gtest.h>

#include <atomic>
#include <thread>

using namespace std::chrono_literals;

namespace
{
    class LoggerTestFixture :
        public testing::Test
    {
    };

} // namespace

TEST_F(LoggerTestFixture, TODO)
{
    FAIL() << "TODO";
}
