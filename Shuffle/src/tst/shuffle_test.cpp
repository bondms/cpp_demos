#include "../app/shuffle.h"

#include <gtest/gtest.h>

#include <string>

namespace
{
    class ShuffleTestFixture :
        public ::testing::Test
    {
    };

    template<typename Shuffler>
    void test(int max)
    {
        std::set<int> s{};
        Shuffler shuffler(max);
        for(auto i = 0 ; i < (max * 2 + max / 2 + 1) ; ++i)
        {
            if ( 0 == i % (max + 1) )
            {
                s.clear();
            }
            EXPECT_TRUE(s.insert(shuffler()).second);
        }
    }
} // namespace

TEST_F(ShuffleTestFixture, Simple_Quick)
{
    test<Shuffle::Simple<int>>(9);
}

TEST_F(ShuffleTestFixture, LowMem_Quick)
{
    test<Shuffle::LowMem<int>>(9);
}

TEST_F(ShuffleTestFixture, Simple_Slow)
{
    test<Shuffle::Simple<int>>(999'999);
}

TEST_F(ShuffleTestFixture, LowMem_Slow)
{
    test<Shuffle::LowMem<int>>(999'999);
}
