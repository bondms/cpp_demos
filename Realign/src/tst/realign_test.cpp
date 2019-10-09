#include "../app/realign.h"

#include <gtest/gtest.h>

#include <iterator>
#include <string>

namespace
{
    struct TestParameter
    {
        const std::wstring input{};
        const std::wstring output{};
        const std::size_t pos{};
    };

    TestParameter test_parameter[] =
    {
        {
            L"",
            L"",
            0,
        },
        {
            L" ",
            L" ",
            1,
        },
        {
            L"a",
            L"a",
            0,
        },
        {
            L"a ",
            L" a",
            1,
        },
        {
            L"ab  ",
            L"  ab",
            2,
        },
        {
            L"a b ",
            L" a b",
            1,
        },
        {
            L"a b    ",
            L"    a b",
            4,
        },
        {
            L" ab  ",
            L"   ab",
            2,
        },
        {
            L"   ab  ",
            L"     ab",
            2,
        },
    };

    class TestFixture :
        public ::testing::TestWithParam<TestParameter>
    {
    };
} // namespace

INSTANTIATE_TEST_CASE_P(simple, TestFixture, ::testing::ValuesIn(test_parameter));

TEST_P(TestFixture, simple)
{
    const auto & param{ GetParam() };

    auto data{ param.input };
    // const auto ret{ realign(data.begin(), data.end()) };
    realign(data);

    const auto & expected{ param.output };

    EXPECT_EQ(expected, data);
    // EXPECT_EQ(param.pos, std::distance(data.begin(), ret));
}
