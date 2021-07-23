// Copyright 2021 Mark Bond

#include "lib/encode.h"

#include <gmock/gmock.h>

namespace
{

struct Parameters {
    Parameters(size_t _input, std::vector<uint8_t> _expected) :
        input{_input},
        expected{_expected}
    {
    }

    size_t input;
    std::vector<uint8_t> expected;
};

class EncodeTestFixture :
    public testing::TestWithParam<Parameters>
{
};

} // namespace

TEST_P(EncodeTestFixture, Simple) {
    auto param{ GetParam() };
    EXPECT_EQ(param.expected, Encode(param.input));
}

Parameters parameterValues[] = {
    Parameters(size_t{0}, std::vector<uint8_t>{0}),
    Parameters(size_t{1}, std::vector<uint8_t>{1}),
    Parameters(size_t{2}, std::vector<uint8_t>{2}),
    Parameters(size_t{0x7F}, std::vector<uint8_t>{0x7F}),
    Parameters(size_t{0x80}, std::vector<uint8_t>{0x80, 0x80}),
    Parameters(size_t{0x81}, std::vector<uint8_t>{0x80, 0x81}),
    Parameters(size_t{0xFF}, std::vector<uint8_t>{0x80, 0xFF}),
    Parameters(size_t{0x100}, std::vector<uint8_t>{0x81, 0x00}),
    Parameters(size_t{0x101}, std::vector<uint8_t>{0x81, 0x01}),
    Parameters(size_t{0x3FFF}, std::vector<uint8_t>{0xBF, 0xFF}),
    Parameters(size_t{0x4000}, std::vector<uint8_t>{0xC0, 0x40, 0x00}),
    Parameters(size_t{0x0200AB}, std::vector<uint8_t>{0xC2, 0x00, 0xAB}),
    Parameters(size_t{0x02AB00}, std::vector<uint8_t>{0xC2, 0xAB, 0x00}),
    Parameters(size_t{0x020000}, std::vector<uint8_t>{0xC2, 0x00, 0x00}),
    Parameters(size_t{0x1200AB}, std::vector<uint8_t>{0xD2, 0x00, 0xAB}),
    Parameters(size_t{0x1FFFFF}, std::vector<uint8_t>{0xDF, 0xFF, 0xFF}),
    Parameters(size_t{0x200000}, std::vector<uint8_t>{0xE0, 0x20, 0x00, 0x00}),
    Parameters(size_t{0xFFFFFFF}, std::vector<uint8_t>{0xEF, 0xFF, 0xFF, 0xFF})
};

INSTANTIATE_TEST_SUITE_P(EncodeTestInstantiation, EncodeTestFixture, testing::ValuesIn(parameterValues));
