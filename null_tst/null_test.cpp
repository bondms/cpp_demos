// Copyright 2021 Mark Bond

#include <gmock/gmock.h>

TEST(NullTest, NullTest)
{
    // There are no real tests for this project.
    // But Bazel requires at least one test in order for `bazel test ...` to succeed.
    EXPECT_TRUE(true);
}
