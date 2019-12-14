#include "../app/Logger.h"

#include "gtest_helper.h"

#include <chrono>
#include <experimental/filesystem>
#include <fstream>
#include <regex>
#include <string>

using namespace std::chrono_literals;

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
    std::string first_log_line(const fs::path& path)
    {
        std::ifstream ifs{path};
        std::string result{};
        std::getline(ifs, result);
        return result;
    }

    auto make_time_point(int year, int month, int day, int hour, int minute, int second, int millisecond)
    {
        std::tm tm{};
        tm.tm_year = year - 1900;
        tm.tm_mon = month - 1;
        tm.tm_mday = day;
        tm.tm_hour = hour;
        tm.tm_min = minute;
        tm.tm_sec = second;

        auto tt{std::mktime(&tm)};

        return std::chrono::system_clock::from_time_t(tt) + std::chrono::milliseconds(millisecond);
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
            // TODO(Will this work on Windows, or will the log file need to be closed?)
            fs::remove_all(temp_test_path);
        }

    protected:
        fs::path temp_test_path{fs::temp_directory_path() / OS_TEXT("LoggerTest")};
    };
}

TEST_F(LoggerTestFixture, Severity_AsString)
{
    EXPECT_EQ("ERR", AsString(Logger::Severity::Error));
    EXPECT_EQ("WRN", AsString(Logger::Severity::Warning));
    EXPECT_EQ("INF", AsString(Logger::Severity::Info));
    EXPECT_EQ("DBG", AsString(Logger::Severity::Debug));
}

TEST_F(LoggerTestFixture, MessageTimeStamp)
{
    EXPECT_EQ("2019-12-14T13:21:12.123Z", Logger::MessageTimeStamp(make_time_point(2019, 12, 14, 13, 21, 12, 123)));
    EXPECT_EQ("2019-01-02T01:02:03.003Z", Logger::MessageTimeStamp(make_time_point(2019, 1, 2, 1, 2, 3, 3)));
}

TEST_F(LoggerTestFixture, FileNameTimeStamp)
{
    EXPECT_EQ("20191214_132112", Logger::FileNameTimeStamp(make_time_point(2019, 12, 14, 13, 21, 12, 123)));
    EXPECT_EQ("20190102_010203", Logger::FileNameTimeStamp(make_time_point(2019, 1, 2, 1, 2, 3, 3)));
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
    GtestHelper::expect_throw_with_callback<std::runtime_error>(
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
    std::regex re{R"(\d{4}\-\d{2}\-\d{2}T\d{2}:\d{2}:\d{2}\.\d{3}Z ERR >> Test message)"};
    EXPECT_TRUE(std::regex_match(logged_message, re));
}

TEST_F(LoggerTestFixture, TODO)
{
    FAIL() << "TODO: Write more tests";
}
