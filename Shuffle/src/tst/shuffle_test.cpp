#include "../app/shuffle.h"

#include <gtest/gtest.h>

#include <cstdlib>

namespace
{
    class ShuffleTestFixture :
        public ::testing::Test
    {
    };

    template<typename Shuffler>
    void test(Shuffler & shuffler)
    {
        auto max = shuffler.max();
        std::set<int> s{};
        for ( auto i = 0 ; i < (max * 2 + max / 2 + 1) ; ++i )
        {
            if ( 0 == i % (max + 1) )
            {
                s.clear();
            }
            EXPECT_TRUE(s.insert(shuffler()).second);
        }
    }
} // namespace

TEST_F(ShuffleTestFixture, Simple_ShuffleMillion)
{
    Shuffle::Simple<int> shuffler{999'999};
    test(shuffler);
}

TEST_F(ShuffleTestFixture, Simple_FullRangeOfType)
{
    Shuffle::Simple<std::uint8_t> shuffler{std::numeric_limits<std::uint8_t>::max()};
    test(shuffler);
}

TEST_F(ShuffleTestFixture, Simple_Zero)
{
    Shuffle::Simple<int> shuffler{0};
    EXPECT_EQ(0, shuffler());
    EXPECT_EQ(0, shuffler());
}

TEST_F(ShuffleTestFixture, Simple_Negative)
{
    EXPECT_THROW(Shuffle::Simple<int>{static_cast<std::vector<int>::size_type>(-1)}, std::logic_error);
}

TEST_F(ShuffleTestFixture, Simple_MaxLimit)
{
    EXPECT_THROW(Shuffle::Simple<std::size_t>{std::numeric_limits<std::size_t>::max()}, std::logic_error);
    EXPECT_NO_THROW(Shuffle::Simple<std::size_t>{std::numeric_limits<std::size_t>::max() - 1});
}

TEST_F(ShuffleTestFixture, LowMem_ShuffleMillion)
{
    Shuffle::LowMem<int, 999'999> shuffler{};
    test(shuffler);
}

TEST_F(ShuffleTestFixture, LowMem_FullRangeOfType)
{
    Shuffle::LowMem<std::uint8_t, std::numeric_limits<std::uint8_t>::max()> shuffler{};
    test(shuffler);
}

TEST_F(ShuffleTestFixture, LowMem_Zero)
{
    Shuffle::LowMem<int, 0> shuffler{};
    EXPECT_EQ(0, shuffler());
    EXPECT_EQ(0, shuffler());
}

TEST_F(ShuffleTestFixture, LowMem_MaxLimit)
{
    EXPECT_NO_THROW(Shuffle::Simple<std::size_t>{std::numeric_limits<std::size_t>::max() - 1});
}
