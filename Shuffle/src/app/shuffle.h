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
    std::vector<T> v_{};
    const std::size_t max_;

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
    std::random_device rd_{};
    std::vector<std::uint8_t> v_;
    std::mt19937 eng_;
    std::uniform_int_distribution<int> dist_;

public:
    LowMem(int max) :
        v_(max / 8 + ( 0 == max % 8 ? 0 : 1 ), 0),
        eng_{rd_()},
        dist_{0, max}
    {
    }

    T operator()()
    {
        auto r = dist_(eng_);
        // TODO(MarkBond): Implement.
        return r;
    }
};

} // namespace Shuffle
