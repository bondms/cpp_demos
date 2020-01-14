#include "Encode.h"

// TODO(MarkBond): Just for testing.
#include <iomanip>
#include <iostream>

#include <climits>

static_assert(0 == (CHAR_BIT % 8));

namespace
{
    template<typename T>
    auto shift_left_byte(T& v)
    {
        constexpr auto bits_in_t{ sizeof(T) * CHAR_BIT };
        uint8_t result = v >> (bits_in_t - 8);
        v = v << 8;
        return result;
    }
} // namespace

std::vector<uint8_t> Encode(size_t input)
{
    std::vector<uint8_t> result{};

    // size_t mask{ ~(static_cast<size_t>(-1) >> 8) };
    // int shifts{ 0 };

    // while (mask != 0)
    // {
    //     std::cout << "*** " << std::hex << mask << " ***" << std::endl;

    //     if ( (mask & input) != 0 )
    //     {
    //         break;
    //     }

    //     mask = mask >> 8;
    //     ++shifts;
    // }

    // while (mask != 0)
    // {
    //     std::cout << "*** " << std::hex << mask << " ***" << std::endl;

    //     xxx;

    //     mask = mask >> 8;
    //     ++shifts;
    // }

    uint8_t currentByte{ 0 };

    std::cout << "*** First loop ***" << std::endl;
    while ( (0 == currentByte) && (0 != input) )
    {
        currentByte = shift_left_byte(input);
        std::cout << "*** currentByte: " << std::hex << int(currentByte) << " ***" << std::endl;
        std::cout << "*** input: " << std::hex << input << " ***" << std::endl;
    }

    std::cout << "*** Second loop ***" << std::endl;
    while ( (0 != currentByte) || (0 != input) )
    {
        result.push_back(currentByte);
        currentByte = shift_left_byte(input);
        std::cout << "*** currentByte: " << std::hex << int(currentByte) << " ***" << std::endl;
        std::cout << "*** input: " << std::hex << input << " ***" << std::endl;
    }

    return result;
}
