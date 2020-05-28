#pragma once

#include <algorithm>
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
        if(0 == max_)
        {
            throw std::runtime_error{"Invalid size"};
        }
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

} // namespace Shuffle
