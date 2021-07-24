// Copyright 2021 Mark Bond

#include <chrono>
#include <filesystem>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

#include "lib/logger.h"

#include "lib/gtest_helper.h"
#include "lib/os.h"

using std::chrono_literals::operator""s;
using std::string_literals::operator""s;

namespace {

auto get_log_lines(const std::filesystem::path& path) {
    std::ifstream ifs{ path };
    ifs.exceptions(std::ifstream::badbit);
    std::vector<std::string> result{};
    std::string line{};
    while ( ifs ) {
        std::getline(ifs, line);
        result.push_back(line);
    }
    if ( !result.empty() ) {
        EXPECT_TRUE(result.back().empty());
        result.pop_back();
    }
    return result;
}

auto make_time_point(
        int year, int month, int day,
        int hour, int minute, int second, int millisecond) {
    std::tm tm{};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;

    auto tt{ std::mktime(&tm) };

    return std::chrono::system_clock::from_time_t(tt)
        + std::chrono::milliseconds(millisecond);
}

constexpr auto log_message_prefix_regex{
    R"(\d{4}\-\d{2}\-\d{2}T\d{2}:\d{2}:\d{2}\.\d{3}Z \d{7}:\d{15} )"
};

class LoggerTestFixture :
        public testing::Test {
    void SetUp() override {
        std::filesystem::create_directory(temp_test_path_);
    }

    void TearDown() override {
        Logger::Close(Logger::ErrorReporting::throwOnError);
        std::filesystem::remove_all(temp_test_path_);
    }

 protected:
    std::filesystem::path temp_test_path_{
        std::filesystem::temp_directory_path() / OS_TEXT("LoggerTest")
    };

    auto get_log_file_path() {
        std::string test_name{
            ::testing::UnitTest::GetInstance()->current_test_info()->name()
        };
        return temp_test_path_ / (test_name + ".log");
    }
};

}  // namespace

TEST_F(LoggerTestFixture, Severity_AsString) {
    EXPECT_STREQ("ERR", AsString(Logger::Severity::Error));
    EXPECT_STREQ("WRN", AsString(Logger::Severity::Warning));
    EXPECT_STREQ("INF", AsString(Logger::Severity::Info));
    EXPECT_STREQ("DBG", AsString(Logger::Severity::Debug));
}

TEST_F(LoggerTestFixture, MessageTimeStamp) {
    EXPECT_EQ(
        "2019-12-14T13:21:12.123Z",
        Logger::MessageTimeStamp(
            make_time_point(2019, 12, 14, 13, 21, 12, 123)));
    EXPECT_EQ(
        "2019-01-02T01:02:03.003Z",
        Logger::MessageTimeStamp(make_time_point(2019, 1, 2, 1, 2, 3, 3)));
}

TEST_F(LoggerTestFixture, FileNameTimeStamp) {
    EXPECT_EQ(
        OS_TEXT("20191214_132112"),
        Logger::FileNameTimeStamp(
            make_time_point(2019, 12, 14, 13, 21, 12, 123)));
    EXPECT_EQ(
        OS_TEXT("20190102_010203"),
        Logger::FileNameTimeStamp(make_time_point(2019, 1, 2, 1, 2, 3, 3)));
}

TEST_F(LoggerTestFixture, Initialise_Simple) {
    auto log_file_path{ get_log_file_path() };
    EXPECT_FALSE(std::filesystem::exists(log_file_path));
    Logger::Initialise(log_file_path);
    Logger::Close(Logger::ErrorReporting::throwOnError);
    EXPECT_TRUE(std::filesystem::exists(log_file_path));
    EXPECT_EQ(0, std::filesystem::file_size(log_file_path));
}

TEST_F(LoggerTestFixture, Initialise_Failure) {
    GtestHelper::expect_throw_with_callback<std::runtime_error>(
        []{ Logger::Initialise("/invalid/path/to/file.txt"); },
        [](const std::runtime_error& e){
            return std::string {
                    "Failed to open log file: /invalid/path/to/file.txt"
                } == e.what();
        });
}

TEST_F(LoggerTestFixture, LOG_ERROR_Simple) {
    auto log_file_path{get_log_file_path()};
    Logger::Initialise(log_file_path);
    LOG_ERROR("Test message");
    Logger::Close(Logger::ErrorReporting::throwOnError);
    auto lines{get_log_lines(log_file_path)};
    ASSERT_EQ(1, lines.size());
    std::regex re{log_message_prefix_regex + "ERR >> Test message"s};
    if ( !std::regex_match(lines[0], re) ) {
        ADD_FAILURE() << "Logged message did not match expected: " << lines[0];
    }
}

TEST_F(LoggerTestFixture, LOG_WARNING_Simple) {
    auto log_file_path{ get_log_file_path() };
    Logger::Initialise(log_file_path);
    LOG_WARNING("Test message");
    Logger::Close(Logger::ErrorReporting::throwOnError);
    auto lines{get_log_lines(log_file_path)};
    ASSERT_EQ(1, lines.size());
    std::regex re{log_message_prefix_regex + "WRN >> Test message"s};
    if ( !std::regex_match(lines[0], re) ) {
        ADD_FAILURE() << "Logged message did not match expected: " << lines[0];
    }
}

TEST_F(LoggerTestFixture, LOG_INFO_Simple) {
    auto log_file_path{ get_log_file_path() };
    Logger::Initialise(log_file_path);
    LOG_INFO("Test message");
    Logger::Close(Logger::ErrorReporting::throwOnError);
    auto lines{get_log_lines(log_file_path)};
    ASSERT_EQ(1, lines.size());
    std::regex re{log_message_prefix_regex + "INF >> Test message"s};
    if ( !std::regex_match(lines[0], re) ) {
        ADD_FAILURE() << "Logged message did not match expected: " << lines[0];
    }
}

TEST_F(LoggerTestFixture, LOG_DEBUG_Simple) {
    auto log_file_path{ get_log_file_path() };
    Logger::Initialise(log_file_path);
    LOG_DEBUG("Test message");
    Logger::Close(Logger::ErrorReporting::throwOnError);
    auto lines{ get_log_lines(log_file_path) };
#ifdef _DEBUG
    ASSERT_EQ(1, lines.size());
    std::regex re{ log_message_prefix_regex + "DBG >> Test message"s };
    if ( !std::regex_match(lines[0], re) ) {
        ADD_FAILURE() << "Logged message did not match expected: " << lines[0];
    }
#else
    EXPECT_TRUE(lines.empty());
#endif
}

TEST_F(LoggerTestFixture, LOG_messages_append) {
    auto log_file_path{ get_log_file_path() };
    Logger::Initialise(log_file_path);
    LOG_INFO("Test message 1");
    LOG_INFO("Test message 2");
    Logger::Close(Logger::ErrorReporting::throwOnError);
    auto lines{get_log_lines(log_file_path)};
    ASSERT_EQ(2, lines.size());
    {
        std::regex re{ log_message_prefix_regex + "INF >> Test message 1"s };
        if ( !std::regex_match(lines[0], re) ) {
            ADD_FAILURE()
                << "First logged message did not match expected: " << lines[0];
        }
    }
    {
        std::regex re{ log_message_prefix_regex + "INF >> Test message 2"s };
        if ( !std::regex_match(lines[1], re) ) {
            ADD_FAILURE()
                << "First logged message did not match expected: " << lines[1];
        }
    }
}

TEST_F(LoggerTestFixture, LOG_appends_to_existing_file) {
    auto log_file_path{ get_log_file_path() };

    {
        std::ofstream ofs{log_file_path};
        ofs << "Test message 1\n";
    }

    Logger::Initialise(log_file_path);
    LOG_INFO("Test message 2");
    Logger::Close(Logger::ErrorReporting::throwOnError);

    auto lines{get_log_lines(log_file_path)};
    ASSERT_EQ(2, lines.size());

    EXPECT_EQ("Test message 1", lines[0]);

    std::regex re{ log_message_prefix_regex + "INF >> Test message 2"s };
    if ( !std::regex_match(lines[1], re) ) {
        ADD_FAILURE()
            << "First logged message did not match expected: " << lines[1];
    }
}

TEST_F(LoggerTestFixture, Rotate) {
    auto log_file_path{ get_log_file_path() };
    Logger::Initialise(log_file_path, 5);
    LOG_INFO("123456");
    Logger::Flush(Logger::ErrorReporting::throwOnError);
    LOG_INFO("123456");
    Logger::Close(Logger::ErrorReporting::throwOnError);

    EXPECT_TRUE(std::filesystem::is_regular_file(get_log_file_path()));

    std::vector<size_t> file_sizes{};
    for ( auto& p : std::filesystem::directory_iterator(temp_test_path_) ) {
        file_sizes.push_back(std::filesystem::file_size(p));
    }

    ASSERT_EQ(2, file_sizes.size());
    EXPECT_EQ(file_sizes[0], file_sizes[1]);
}

TEST_F(LoggerTestFixture, Escaping) {
    auto log_file_path{ get_log_file_path() };
    Logger::Initialise(log_file_path);
    LOG_INFO("Test message\n");
    Logger::Close(Logger::ErrorReporting::throwOnError);
    auto lines{get_log_lines(log_file_path)};
    ASSERT_EQ(1, lines.size());
    std::regex re{
        log_message_prefix_regex + R"<<<(INF >> Test message\\n)<<<"s
    };
    if ( !std::regex_match(lines[0], re) ) {
        ADD_FAILURE() << "Logged message did not match expected: " << lines[0];
    }
}

TEST_F(LoggerTestFixture, Benchmark) {
    auto log_file_path{ get_log_file_path() };
    Logger::Initialise(log_file_path);
    for ( auto i = 0 ; i < 100000 ; ++i ) {
        LOG_INFO("A typical log message with no escaping required");
    }
}
