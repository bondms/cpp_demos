// Copyright 2021 Mark Bond

#include <gmock/gmock.h>

#include <string>

#include "lib/pool.h"

using MockPool = Pool<std::string>;

TEST(PoolTest, EmptyPool) {
    MockPool pool{};
    EXPECT_FALSE(pool.availableToStart());
    EXPECT_THROW(pool.nextToStart(), std::exception);

    bool pred_fn_called{ false };
    auto pred_fn = [&](const MockPool::ContainerItem &) {
        pred_fn_called = true;
        return true;
    };

    EXPECT_THROW(pool.find_if(pred_fn), std::exception);
    EXPECT_FALSE(pred_fn_called);
}

TEST(PoolTest, SingleItem) {
    MockPool pool{};

    auto it{ pool.add("MyJob") };
    EXPECT_EQ("MyJob", it->jobData);
    EXPECT_EQ(State::initial, it->jobState);

    EXPECT_TRUE(pool.availableToStart());
    EXPECT_EQ("MyJob", pool.nextToStart());

    bool pred_fn_called{ false };
    auto pred_fn = [&](const MockPool::ContainerItem & containerItem) {
        EXPECT_FALSE(pred_fn_called);
        EXPECT_EQ("MyJob", containerItem.jobData);
        EXPECT_EQ(State::initial, containerItem.jobState);
        return true;
    };

    EXPECT_EQ("MyJob", pool.find_if(pred_fn));
    EXPECT_TRUE(pred_fn_called);
}

// TODO(MarkBond): Implement more tests.
