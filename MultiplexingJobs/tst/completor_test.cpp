// Copyright 2021 Mark Bond

#include <gmock/gmock.h>

#include <string>

#include "lib/completor.h"

TEST(CompletorTest, Simple) {
    Sync<std::string> sync{};

    auto completeFunc = [&](std::string&, int&){
        return true;
    };

    auto jobMatchFunc = [&](const std::string&, const int&){
        return true;
    };

    Completor<std::string, int> completor{sync, completeFunc, jobMatchFunc};
}

// TODO(MarkBond): Implement more tests.
