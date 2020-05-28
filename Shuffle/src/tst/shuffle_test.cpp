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

TEST_F(ShuffleTestFixture, Simple)
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

TEST_F(ShuffleTestFixture, LowMem)
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
