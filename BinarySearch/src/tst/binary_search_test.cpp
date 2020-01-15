#include "../app/binary_search.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <iterator>

namespace
{
    struct TestParameter
    {
        const std::array<int, 3> data{};
        const int item{};
    };

    TestParameter test_parameter[] =
    {
        {
            { 1, 2, 3, },
            0,
        },
        {
            { 1, 2, 3, },
            1,
        },
        {
            { 1, 2, 3, },
            2,
        },
        {
            { 1, 2, 3, },
            3,
        },
        {
            { 1, 2, 3, },
            4,
        },
    };

    class TestFixture :
        public ::testing::TestWithParam<TestParameter>
    {
    };
} // namespace

INSTANTIATE_TEST_SUITE_P(simple, TestFixture, ::testing::ValuesIn(test_parameter));

TEST_P(TestFixture, simple)
{
    const auto & param{ GetParam() };

    const auto actual{ binary_search(std::cbegin(param.data), std::cend(param.data), param.item) };
    const auto expected { std::lower_bound(std::cbegin(param.data), std::cend(param.data), param.item) };

    EXPECT_EQ(expected, actual);
}
