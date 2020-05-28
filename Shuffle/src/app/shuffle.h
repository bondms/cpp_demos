#pragma once

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <random>

namespace Shuffle
{

template<typename T>
class Simple
{
    const std::size_t max_;
    std::vector<T> v_{};

public:
    Simple(std::size_t max) :
        max_{max}
    {
    }

    T operator()()
    {
        if( v_.empty() )
        {
            v_.resize(max_+1);
            std::iota(v_.begin(), v_.end(), 0);
            std::random_shuffle(v_.begin(), v_.end());
        }
        auto result = v_.back();
        v_.pop_back();
        return result;
    }
};

template<typename T>
class LowMem
{
    const std::size_t max_;
    std::vector<std::uint8_t> v_{};

public:
    LowMem(std::size_t max) :
        max_{max}
    {
    }

    T operator()()
    {
        // TODO(MarkBond): Implement.
        return 0;
    }
};

} // namespace Shuffle
