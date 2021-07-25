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
    EXPECT_FALSE(pool.availableToStart());

    bool pred_fn_called{ false };
    auto pred_fn_1 = [&](const MockPool::ContainerItem & containerItem) {
        EXPECT_FALSE(pred_fn_called);
        pred_fn_called = true;
        EXPECT_EQ("MyJob", containerItem.jobData);
        EXPECT_EQ(State::initial, containerItem.jobState);
        return true;
    };

    EXPECT_EQ("MyJob", pool.find_if(pred_fn_1));
    EXPECT_TRUE(pred_fn_called);

    pool.erase(it);
    pred_fn_called = false;
    auto pred_fn_2 = [&](const MockPool::ContainerItem &) {
        pred_fn_called = true;
        return true;
    };

    EXPECT_THROW(pool.find_if(pred_fn_2), std::exception);
    EXPECT_FALSE(pred_fn_called);
}

TEST(PoolTest, SeveralItems) {
    MockPool pool{};

    auto it1{ pool.add("FirstJob") };
    auto it2{ pool.add("SecondJob") };

    EXPECT_EQ("FirstJob", it1->jobData);
    EXPECT_EQ("SecondJob", it2->jobData);

    EXPECT_TRUE(pool.availableToStart());
    EXPECT_EQ("FirstJob", pool.nextToStart());

    EXPECT_TRUE(pool.availableToStart());
    EXPECT_EQ("SecondJob", pool.nextToStart());

    EXPECT_FALSE(pool.availableToStart());
}
