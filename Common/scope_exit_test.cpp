#include "lib/scope_exit.h"

#include "gtest_helper.h"

namespace
{
    class ScopeExitTestFixture :
        public testing::Test
    {
    };
}

TEST_F(ScopeExitTestFixture, ScopeExitsCleanly)
{
    bool b = false;
    {
        auto se = make_scope_exit([&](){ b = true; });
        EXPECT_FALSE(b);
    }
    EXPECT_TRUE(b);
}

TEST_F(ScopeExitTestFixture, ScopeExitsOnException)
{
    bool b = false;
    try
    {
        auto se = make_scope_exit([&](){ b = true; });
        EXPECT_FALSE(b);
        throw std::runtime_error{"test"};
    }
    catch ( const std::exception & )
    {
        EXPECT_TRUE(b);
    }
    EXPECT_TRUE(b);
}
