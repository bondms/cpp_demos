#include "../app/shuffle.h"

#include <gtest/gtest.h>

#include <string>

namespace
{
    class ShuffleTestFixture :
        public ::testing::Test
    {
    };
} // namespace

TEST_F(ShuffleTestFixture, Simple_Quick)
{
    std::set<int> s{};
    Shuffle::Simple<int> simple(9);
    for(auto i = 0 ; i < 25 ; ++i)
    {
        if ( 0 == i % 10 )
        {
            s.clear();
        }
        EXPECT_TRUE(s.insert(simple()).second);
    }
}

TEST_F(ShuffleTestFixture, LowMem_Quick)
{
    std::set<int> s{};
    Shuffle::LowMem<int> lowmem(9);
    for(auto i = 0 ; i < 25 ; ++i)
    {
        if ( 0 == i % 10 )
        {
            s.clear();
        }
        EXPECT_TRUE(s.insert(lowmem()).second);
    }
}

TEST_F(ShuffleTestFixture, Simple_Slow)
{
    std::set<int> s{};
    Shuffle::Simple<int> simple(99'999);
    for(auto i = 0 ; i < 250'000 ; ++i)
    {
        if ( 0 == i % 100'000 )
        {
            s.clear();
        }
        EXPECT_TRUE(s.insert(simple()).second);
    }
}

TEST_F(ShuffleTestFixture, LowMem_Slow)
{
    std::set<int> s{};
    Shuffle::LowMem<int> lowmem(99'999);
    for(auto i = 0 ; i < 250'000 ; ++i)
    {
        if ( 0 == i % 100'000 )
        {
            s.clear();
        }
        EXPECT_TRUE(s.insert(lowmem()).second);
    }
}
