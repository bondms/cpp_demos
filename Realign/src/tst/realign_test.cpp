#include "../app/realign.h"

#include <gtest/gtest.h>

#include <string>

namespace
{
    struct TestParameter
    {
        const std::wstring input{};
        const std::wstring output{};
        const std::wstring value{};
    };

    TestParameter test_parameters[] =
    {
        {
            L"",
            L"",
            L"",
        },
        {
            L" ",
            L" ",
            L"",
        },
        {
            L"a",
            L"a",
            L"a",
        },
        {
            L"a ",
            L" a",
            L"a",
        },
        {
            L"ab  ",
            L"  ab",
            L"ab",
        },
        {
            L"a b ",
            L" a b",
            L"a b",
        },
        {
            L"a b    ",
            L"    a b",
            L"a b",
        },
        {
            L" ab  ",
            L"   ab",
            L" ab",
        },
        {
            L"   ab  ",
            L"     ab",
            L"   ab",
        },
    };

    class TestFixture :
        public ::testing::TestWithParam<TestParameter>
    {
    };
} // namespace

INSTANTIATE_TEST_SUITE_P(simple, TestFixture, ::testing::ValuesIn(test_parameters));

TEST_P(TestFixture, simple)
{
    const auto & param{ GetParam() };

    auto data{ param.input };
    const auto ret{ realign(data.begin(), data.end()) };

    const auto & expected{ param.output };

    EXPECT_EQ(expected, data);
    EXPECT_EQ(param.value, std::wstring(ret, data.end()));
}
