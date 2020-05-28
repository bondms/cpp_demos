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
        const T max_;

    public:
        Simple(T max) :
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

        T max() const
        {
            return max_;
        }
    };

    template<typename T, std::size_t max_>
    class LowMem
    {
        std::random_device rd_{};
        std::bitset<max_+1> bitset_{};
        int remaining_;
        std::mt19937 eng_;
        std::uniform_int_distribution<T> dist_;

    public:
        LowMem() :
            remaining_{max_ + 1},
            eng_{rd_()},
            dist_{0, max_}
        {
        }

        T operator()()
        {
            if ( 0 == remaining_ )
            {
                remaining_ = max_ + 1;
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

        constexpr T max() const
        {
            return max_;
        }
    };
} // namespace Shuffle
