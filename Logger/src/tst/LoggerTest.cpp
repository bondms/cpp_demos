#include "../app/Logger.h"

#include <gtest/gtest.h>

#include <experimental/filesystem>
#include <functional>

namespace fs = std::experimental::filesystem;

// TODO(Move somewhere else)
#ifdef __linux__ 
    #define OS_TEXT(text) text
#elif defined(_WIN32)
    #define OS_TEXT(text) L##text
#else
    static_assert(false, "Unrecognised OS");
#endif

// TODO(Move somewhere else)
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

namespace
{
    class LoggerTestFixture :
        public testing::Test
    {
        void SetUp() override
        {
            fs::create_directory(temp_test_path);
        }
        
        void TearDown() override
        {
            fs::remove_all(temp_test_path);
        }

    protected:
        fs::path temp_test_path{fs::temp_directory_path() / OS_TEXT("LoggerTest")};
    };
}

TEST_F(LoggerTestFixture, Initialise_Simple)
{
    auto log_file_path{temp_test_path / OS_TEXT("CreatesLogFile.log")};
    EXPECT_FALSE(fs::exists(log_file_path));
    Logger::Initialise(log_file_path);
    EXPECT_TRUE(fs::exists(log_file_path));
    EXPECT_EQ(0, fs::file_size(log_file_path));
}

TEST_F(LoggerTestFixture, Initialise_Failure)
{
    expect_throw_with_callback<std::runtime_error>(
        []{Logger::Initialise("/invalid/path/to/file.txt");},
        [](const std::runtime_error& e){ return std::string{"Failed to open log file: /invalid/path/to/file.txt"} == e.what(); }
        );
}

TEST_F(LoggerTestFixture, TODO)
{
    FAIL() << "TODO: Write more tests";
}
