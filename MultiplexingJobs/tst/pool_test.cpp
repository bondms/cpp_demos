// Copyright 2021 Mark Bond

#include <gmock/gmock.h>

#include <string>

#include "lib/pool.h"

using MockPool = Pool<std::string>;

TEST(PoolTest, EmptyPool) {
    MockPool pool{};
    EXPECT_FALSE(pool.availableToStart());
    EXPECT_THROW(pool.nextToStart(), std::exception);

    auto pred_fn = [&](const MockPool::ContainerItem &) {
        ADD_FAILURE() << "pred_fn called unexpectedly";
        return true;
    };

    EXPECT_THROW(pool.find_if(pred_fn), std::exception);
}

TEST(PoolTest, SingleItem) {
    MockPool pool{};

    auto it{ pool.add("MyJob") };
    EXPECT_EQ("MyJob", it->data);
    EXPECT_EQ(State::initial, it->state);

    EXPECT_TRUE(pool.availableToStart());
    EXPECT_EQ("MyJob", pool.nextToStart());
    EXPECT_FALSE(pool.availableToStart());

    bool pred_fn_called{ false };
    auto pred_fn_1 = [&](const MockPool::ContainerItem & containerItem) {
        EXPECT_FALSE(pred_fn_called);
        pred_fn_called = true;
        EXPECT_EQ("MyJob", containerItem.data);
        EXPECT_EQ(State::initial, containerItem.state);
        return true;
    };

    const auto containerItemRef{ pool.find_if(pred_fn_1) };
    EXPECT_EQ("MyJob", containerItemRef.data);
    EXPECT_EQ(State::initial, containerItemRef.state);
    EXPECT_TRUE(pred_fn_called);

    pool.erase(it);
    auto pred_fn_2 = [&](const MockPool::ContainerItem &) {
        ADD_FAILURE() << "Unexpected call to pred_fn_2";
        return true;
    };

    EXPECT_THROW(pool.find_if(pred_fn_2), std::exception);
}

TEST(PoolTest, SeveralItems) {
    MockPool pool{};

    auto it1{ pool.add("FirstJob") };
    auto it2{ pool.add("SecondJob") };

    EXPECT_EQ("FirstJob", it1->data);
    EXPECT_EQ("SecondJob", it2->data);

    EXPECT_TRUE(pool.availableToStart());
    EXPECT_EQ("FirstJob", pool.nextToStart());

    EXPECT_TRUE(pool.availableToStart());
    EXPECT_EQ("SecondJob", pool.nextToStart());

    EXPECT_FALSE(pool.availableToStart());
}
