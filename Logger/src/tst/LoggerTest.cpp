#include "../app/Logger.h"

#include <gtest/gtest.h>

#include <functional>

namespace
{
    class LoggerTestFixture :
        public testing::Test
    {
    };
}

template<typename ExceptionType>
void expect_throw_with_callback(std::function<void()> code, std::function<bool(ExceptionType e)> callback)
{
    try
    {
        code();
        ADD_FAILURE() << "Exception not thrown";
    }
    catch ( const ExceptionType& e )
    {
        EXPECT_TRUE(callback(e));
    }
    catch ( ... )
    {
        ADD_FAILURE() << "Unexpected exception type thrown";
    }
}

TEST_F(LoggerTestFixture, ThrowsOnLogFileCreationFailure)
{
    expect_throw_with_callback<std::runtime_error>(
        []{Logger::Initialise("/invalid/path/to/file.txt");},
        [](const std::runtime_error& e){ return std::string{"Failed to open log file: /invalid/path/to/file.txt"} == e.what(); }
        );
}

TEST_F(LoggerTestFixture, TODO)
{
    FAIL() << "TODO";
}
