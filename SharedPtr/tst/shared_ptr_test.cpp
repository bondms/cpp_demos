// Copyright 2021 Mark Bond

#include <gmock/gmock.h>

#include "lib/shared_ptr.h"

using SharedPtr::BadGetWithTimeout;
using SharedPtr::GoodGetWithTimeout;

using std::chrono_literals::operator""s;

namespace {

class SharedPtrTestFixture :
    public testing::Test {
};

}  // namespace

TEST_F(SharedPtrTestFixture, BadWithLongEnoughWait) {
    EXPECT_EQ("The data", BadGetWithTimeout(2s));
}

TEST_F(SharedPtrTestFixture, DISABLED_BadWithTooShortWait) {
    EXPECT_TRUE(BadGetWithTimeout(0s).empty());
}

TEST_F(SharedPtrTestFixture, GoodWithLongEnoughWait) {
    EXPECT_EQ("The data", GoodGetWithTimeout(2s));
}

TEST_F(SharedPtrTestFixture, GoodWithTooShortWait) {
    EXPECT_TRUE(GoodGetWithTimeout(0s).empty());
}
