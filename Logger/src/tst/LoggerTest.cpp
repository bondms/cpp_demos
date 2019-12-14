#include "../app/Logger.h"

#include <gtest/gtest.h>

#include <experimental/filesystem>
#include <fstream>
#include <functional>
#include <regex>
#include <string>

namespace fs = std::experimental::filesystem;

// TODO(Move somewhere else)
#ifdef __linux__ 
    #define OS_TEXT(text) text
#elif defined(_WIN32)
    #define OS_TEXT(text) L##text
#else
    static_assert(false, "Unrecognised OS");
#endif

namespace
{
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

    std::string first_log_line(const fs::path& path)
    {
        std::ifstream ifs{path};
        std::string result{};
        std::getline(ifs, result);
        return result;
    }

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

TEST_F(LoggerTestFixture, Severity_AsString)
{
    EXPECT_EQ("Error", AsString(Logger::Severity::Error));
    EXPECT_EQ("Warning", AsString(Logger::Severity::Warning));
    EXPECT_EQ("Info", AsString(Logger::Severity::Info));
    EXPECT_EQ("Debug", AsString(Logger::Severity::Debug));
}

TEST_F(LoggerTestFixture, Initialise_Simple)
{
    std::string test_name{::testing::UnitTest::GetInstance()->current_test_info()->name()};
    auto log_file_path{temp_test_path / (test_name + ".log")};
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

TEST_F(LoggerTestFixture, LOG_ERROR_Simple)
{
    std::string test_name{::testing::UnitTest::GetInstance()->current_test_info()->name()};
    auto log_file_path{temp_test_path / (test_name + ".log")};
    Logger::Initialise(log_file_path);
    LOG_ERROR("Test message");
    auto logged_message{first_log_line(log_file_path)};
    EXPECT_EQ("xxx", logged_message);
}

TEST_F(LoggerTestFixture, TODO)
{
    FAIL() << "TODO: Write more tests";
}
