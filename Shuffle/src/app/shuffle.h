#pragma once

#include <algorithm>
#include <bitset>
#include <limits>
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
            if ( v_.empty() )
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

    template<typename T, T N>
    class LowMem
    {
        static_assert(N < std::numeric_limits<std::size_t>::max());
        std::mt19937 rand_{std::random_device{}()};
        std::bitset<N + 1> bitset_{};
        std::size_t remaining_;
        std::uniform_int_distribution<T> dist_;

    public:
        LowMem() :
            remaining_{N + 1},
            dist_{0, N}
        {
        }

        T operator()()
        {
            if ( 0 == remaining_ )
            {
                remaining_ = N + 1;
                bitset_.reset();
            }
            --remaining_;
            while ( true )
            {
                auto r = dist_(rand_);
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
            return N;
        }
    };
} // namespace Shuffle
