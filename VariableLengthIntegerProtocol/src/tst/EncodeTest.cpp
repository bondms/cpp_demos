#include "../app/Encode.h"

#include <gtest/gtest.h>

namespace
{
    struct Parameters
    {
        Parameters(size_t input, std::vector<unsigned char> expected) :
            input{input},
            expected{expected}
        {
        }

        size_t input;
        std::vector<unsigned char> expected;
    };

    class EncodeTestFixture :
        public testing::TestWithParam<Parameters>
    {
    };
} // namespace

TEST_P(EncodeTestFixture, Simple)
{
    auto param{ GetParam() };
    EXPECT_EQ(param.expected, Encode(param.input));
}

Parameters parameterValues[] = {
    Parameters(size_t{0}, std::vector<unsigned char>{}),
    Parameters(size_t{1}, std::vector<unsigned char>{1}),
    Parameters(size_t{2}, std::vector<unsigned char>{2}),
    Parameters(size_t{0xFF}, std::vector<unsigned char>{0xFF}),
    Parameters(size_t{0x100}, std::vector<unsigned char>{0x1, 0x0}),
    Parameters(size_t{0x101}, std::vector<unsigned char>{0x1, 0x1}),
    Parameters(size_t{0xFE0087}, std::vector<unsigned char>{0xFE, 0x00, 0x87})
};

INSTANTIATE_TEST_CASE_P(EncodeTestInstantiation, EncodeTestFixture, testing::ValuesIn(parameterValues));
