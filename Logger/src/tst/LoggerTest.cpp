#include "../app/Logger.h"

#include <gtest/gtest.h>

namespace
{
    class LoggerTestFixture :
        public testing::Test
    {
    };
}

TEST_F(LoggerTestFixture, ThrowsOnLogFileCreationFailure)
{
    EXPECT_THROW(Logger::Initialise("/invalid/path/to/file.txt"), std::runtime_error);
}

TEST_F(LoggerTestFixture, TODO)
{
    FAIL() << "TODO";
}
