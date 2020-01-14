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
    Parameters(size_t{0}, std::vector<unsigned char>{0}),
    Parameters(size_t{1}, std::vector<unsigned char>{1}),
    Parameters(size_t{2}, std::vector<unsigned char>{2}),
    Parameters(size_t{0x7F}, std::vector<unsigned char>{0x7F}),
    Parameters(size_t{0x80}, std::vector<unsigned char>{0x80, 0x80}),
    Parameters(size_t{0x81}, std::vector<unsigned char>{0x80, 0x81}),
    Parameters(size_t{0xFF}, std::vector<unsigned char>{0x80, 0xFF}),
    Parameters(size_t{0x100}, std::vector<unsigned char>{0x81, 0x00}),
    Parameters(size_t{0x101}, std::vector<unsigned char>{0x81, 0x01}),
    Parameters(size_t{0x0200AB}, std::vector<unsigned char>{0xC2, 0x00, 0xAB}),
    Parameters(size_t{0x1200AB}, std::vector<unsigned char>{0xD2, 0x00, 0xAB})
};

INSTANTIATE_TEST_CASE_P(EncodeTestInstantiation, EncodeTestFixture, testing::ValuesIn(parameterValues));
