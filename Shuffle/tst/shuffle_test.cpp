// Copyright 2021 Mark Bond

#include <gmock/gmock.h>

#include <cstdlib>

#include "Shuffle/lib/shuffle.h"

namespace {

template <std::int32_t max, typename Shuffler> void test(Shuffler &shuffler) {
  std::bitset<max + 1> bs{};
  for (std::int_fast32_t i = 0; i < (max * 2 + max / 2 + 1); ++i) {
    if (0 == i % (max + 1)) {
      bs.reset();
    }
    const auto v{shuffler()};
    auto r{bs[v]};
    EXPECT_FALSE(r);
    r = true;
  }
}

} // namespace

TEST(ShuffleTest, Simple_ShuffleMillion) {
  Shuffle::Simple<std::int_fast32_t> shuffler{999'999};
  test<999'999>(shuffler);
}

TEST(ShuffleTest, Simple_FullRangeOfType) {
  Shuffle::Simple<std::uint8_t> shuffler{
      std::numeric_limits<std::uint8_t>::max()};
  test<std::numeric_limits<std::uint8_t>::max()>(shuffler);
}

TEST(ShuffleTest, Simple_Zero) {
  Shuffle::Simple<int> shuffler{0};
  EXPECT_EQ(0, shuffler());
  EXPECT_EQ(0, shuffler());
}

TEST(ShuffleTest, Simple_Negative) {
  EXPECT_THROW(Shuffle::Simple<int>{-1}, std::logic_error);
}

TEST(ShuffleTest, LowMem_ShuffleMillion) {
  Shuffle::LowMem<std::int_least32_t, 999'999> shuffler{};
  test<shuffler.max()>(shuffler);
}

TEST(ShuffleTest, LowMem_FullRangeOfType) {
  Shuffle::LowMem<std::uint8_t, std::numeric_limits<std::uint8_t>::max()>
      shuffler{};
  test<shuffler.max()>(shuffler);
}

TEST(ShuffleTest, LowMem_Zero) {
  Shuffle::LowMem<int, 0> shuffler{};
  EXPECT_EQ(0, shuffler());
  EXPECT_EQ(0, shuffler());
}

TEST(ShuffleTest, LowMem_MaxLimit) {
  EXPECT_NO_THROW(Shuffle::Simple<std::size_t>{
      std::numeric_limits<std::size_t>::max() - 1});
}
