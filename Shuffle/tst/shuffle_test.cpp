// Copyright 2021 Mark Bond

#include "lib/shuffle.h"

#include <gmock/gmock.h>

#include <cstdlib>

namespace
{

template<typename Shuffler>
void test(Shuffler & shuffler) {
    const auto max = shuffler.max();
    std::set<std::int_least32_t> s{};
    for ( std::int_fast32_t i = 0 ; i < (max * 2 + max / 2 + 1) ; ++i ) {
        if ( 0 == i % (max + 1) ) {
            s.clear();
        }
        EXPECT_TRUE(s.insert(shuffler()).second);
    }
}

} // namespace

TEST(ShuffleTest, Simple_ShuffleMillion) {
    Shuffle::Simple<std::int_fast32_t> shuffler{999'999};
    test(shuffler);
}

TEST(ShuffleTest, Simple_FullRangeOfType) {
    Shuffle::Simple<std::uint8_t> shuffler{std::numeric_limits<std::uint8_t>::max()};
    test(shuffler);
}

TEST(ShuffleTest, Simple_Zero) {
    Shuffle::Simple<int> shuffler{0};
    EXPECT_EQ(0, shuffler());
    EXPECT_EQ(0, shuffler());
}

TEST(ShuffleTest, Simple_Negative) {
    EXPECT_THROW(Shuffle::Simple<int>{static_cast<std::vector<int>::size_type>(-1)}, std::logic_error);
}

TEST(ShuffleTest, Simple_MaxLimit) {
    EXPECT_THROW(Shuffle::Simple<std::size_t>{std::numeric_limits<std::size_t>::max()}, std::logic_error);
    EXPECT_NO_THROW(Shuffle::Simple<std::size_t>{std::numeric_limits<std::size_t>::max() - 1});
}

TEST(ShuffleTest, LowMem_ShuffleMillion) {
    Shuffle::LowMem<std::int_least32_t, 999'999> shuffler{};
    test(shuffler);
}

TEST(ShuffleTest, LowMem_FullRangeOfType) {
    Shuffle::LowMem<std::uint8_t, std::numeric_limits<std::uint8_t>::max()> shuffler{};
    test(shuffler);
}

TEST(ShuffleTest, LowMem_Zero) {
    Shuffle::LowMem<int, 0> shuffler{};
    EXPECT_EQ(0, shuffler());
    EXPECT_EQ(0, shuffler());
}

TEST(ShuffleTest, LowMem_MaxLimit) {
    EXPECT_NO_THROW(Shuffle::Simple<std::size_t>{std::numeric_limits<std::size_t>::max() - 1});
}
