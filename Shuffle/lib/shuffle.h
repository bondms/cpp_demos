// Copyright 2021 Mark Bond

#pragma once

#include <algorithm>
#include <bitset>
#include <limits>
#include <numeric>
#include <random>
#include <vector>

namespace Shuffle {

template <typename T> class Simple {
  using vec = std::vector<T>;
  std::mt19937 rand_{std::random_device{}()};
  vec v_{};
  const T max_;

public:
  explicit Simple(T max) : max_{max} {
    if (max < 0) {
      throw std::logic_error{"Invalid max (negative)"};
    }
    if (static_cast<size_t>(max) == std::numeric_limits<size_t>::max()) {
      throw std::logic_error{"Invalid max (too large)"};
    }
  }

  T operator()() {
    if (v_.empty()) {
      v_.resize(static_cast<size_t>(max_) + 1);
      std::iota(v_.begin(), v_.end(), 0);
      std::shuffle(v_.begin(), v_.end(), rand_);
    }
    auto result = v_.back();
    v_.pop_back();
    return result;
  }

  T max() const { return max_; }
};

template <typename T, T N> class LowMem {
  static_assert((N >= 0) && (N < std::numeric_limits<std::size_t>::max()));
  std::mt19937 rand_{std::random_device{}()};
  std::bitset<N + 1> bitset_{};
  std::size_t remaining_{N + 1};
  std::uniform_int_distribution<T> dist_{0, N};

public:
  T operator()() {
    if (0 == remaining_) {
      remaining_ = N + 1;
      bitset_.reset();
    }
    --remaining_;
    while (true) {
      auto r = dist_(rand_);
      if (0 != bitset_[static_cast<size_t>(r)]) {
        continue;
      }
      bitset_[static_cast<size_t>(r)] = true;
      return r;
    }
  }

  constexpr T max() const { return N; }
};

} // namespace Shuffle
