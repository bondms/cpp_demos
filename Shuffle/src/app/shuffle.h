#pragma once

#include <algorithm>
#include <bitset>
#include <numeric>
#include <random>
#include <vector>

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

    template<typename T, std::size_t max>
    class LowMem
    {
        std::random_device rd_{};
        std::bitset<max+1> bitset_{};
        int remaining_;
        std::mt19937 eng_;
        std::uniform_int_distribution<T> dist_;

    public:
        LowMem() :
            remaining_{max + 1},
            eng_{rd_()},
            dist_{0, max}
        {
        }

        T operator()()
        {
            if ( 0 == remaining_ )
            {
                remaining_ = max + 1;
                bitset_.reset();
            }
            --remaining_;
            while ( true )
            {
                auto r = dist_(eng_);
                if ( 0 != bitset_[r] )
                {
                    continue;
                }
                bitset_[r] = true;
                return r;
            }
        }
    };
} // namespace Shuffle
