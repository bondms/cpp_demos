// Copyright 2021 Mark Bond

#include <gmock/gmock.h>

#include <string>

#include "lib/completor.h"

TEST(CompletorTest, NoJobs) {
    Sync<std::string> sync{};

    auto completeFunc = [&](std::string&, int&) -> bool {
        throw std::runtime_error{ "Unexpected call to completeFunc" };
    };

    auto jobMatchFunc = [&](const std::string&, const int&) -> bool {
        throw std::runtime_error{ "Unexpected call to jobMatchFunc" };
    };

    Completor<std::string, int> completor{sync, completeFunc, jobMatchFunc};
}
