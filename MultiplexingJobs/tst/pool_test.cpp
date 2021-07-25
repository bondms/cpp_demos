// Copyright 2021 Mark Bond

#include <gmock/gmock.h>

#include <string>

#include "lib/pool.h"

TEST(PoolTest, EmptyPool) {
    Pool<std::string> pool{};
    EXPECT_FALSE(pool.availableToStart());
    EXPECT_THROW(pool.nextToStart(), std::exception);

    bool pred_fn_called{ false };
    auto pred_fn = [&](const Pool<std::string>::ContainerItem &) {
        pred_fn_called = true;
        return true;
    };

    EXPECT_THROW(pool.find_if(pred_fn), std::exception);
    EXPECT_FALSE(pred_fn_called);
}

// TODO(MarkBond): Implement more tests.
