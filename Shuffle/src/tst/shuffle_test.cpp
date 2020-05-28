#include "../app/shuffle.h"

#include <gtest/gtest.h>

#include <string>

namespace
{
    class ShuffleTestFixture :
        public ::testing::Test
    {
    };

    template<int max, typename Shuffler>
    void test(Shuffler & shuffler)
    {
        std::set<int> s{};
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
    Shuffle::Simple<int> simple{9};
    test<9>(simple);
}

TEST_F(ShuffleTestFixture, LowMem_Quick)
{
    Shuffle::LowMem<int, 9> lowMem{};
    test<9>(lowMem);
}

TEST_F(ShuffleTestFixture, Simple_Slow)
{
    Shuffle::Simple<int> simple{999'999};
    test<999'999>(simple);
}

TEST_F(ShuffleTestFixture, LowMem_Slow)
{
    Shuffle::LowMem<int, 999'999> lowMem{};
    test<999'999>(lowMem);
}
