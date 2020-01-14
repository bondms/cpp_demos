#include "Encode.h"

#include <climits>

static_assert(0 == (CHAR_BIT % 8));

namespace
{
    template<typename T>
    auto shift_left_byte(T& v)
    {
        constexpr auto bits_in_t{ sizeof(T) * CHAR_BIT };
        auto result{ static_cast<uint8_t>(v >> (bits_in_t - 8)) };
        v = v << 8;
        return result;
    }
} // namespace

std::vector<uint8_t> Encode(size_t input)
{
    constexpr auto uint8sInInput{ (sizeof(input) * CHAR_BIT) / 8 };

    std::vector<uint8_t> result{};

    uint8_t currentByte{ 0 };
    auto shifts{ 0U };

    while ( shifts < uint8sInInput )
    {
        currentByte = shift_left_byte(input);
        ++shifts;

        if ( 0 != currentByte )
        {
            break;
        }
    }

    auto reservedBits{ static_cast<uint8_t>(uint8sInInput - shifts + 1) };
    auto overlap{ ((~(0xFF >> reservedBits)) & currentByte) != 0 };

    if ( overlap )
    {
        ++reservedBits;
    }

    auto lengthIndicatorBits{ static_cast<uint8_t>(~(0xFF >> (reservedBits - 1))) };

    if ( overlap )
    {
        result.push_back(lengthIndicatorBits);
    }
    else
    {
        currentByte |= lengthIndicatorBits;
    }
    result.push_back(currentByte);

    while ( shifts < uint8sInInput )
    {
        currentByte = shift_left_byte(input);
        ++shifts;

        result.push_back(currentByte);
    }

    return result;
}
