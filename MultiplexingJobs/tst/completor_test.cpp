// Copyright 2021 Mark Bond

#include <gmock/gmock.h>

#include <string>

#include "lib/completor.h"
#include "lib/event.h"

TEST(CompletorTest, EndsOnFailure) {
    Sync<std::string> sync{};
    Event event{ Event::Mode::manualReset, Event::State::nonSignalled };

    bool completeFuncCalled{ false };
    auto completeFunc = [&](std::string&, int&) -> bool {
        EXPECT_FALSE(completeFuncCalled);
        completeFuncCalled = true;
        event.Signal();
        throw std::runtime_error{ "Failure from completeFunc" };
    };

    auto jobMatchFunc = [&](const std::string&, const int&) {
        ADD_FAILURE() << "Unexpected call to jobMatchFunc";
        return true;
    };

    Completor<std::string, int> completor{sync, completeFunc, jobMatchFunc};
    event.Wait();
    EXPECT_TRUE(completeFuncCalled);
}
