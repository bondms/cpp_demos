// Copyright 2021 Mark Bond

#include <gmock/gmock.h>

#include <string>

#include "MultiplexingJobs/lib/completor.h"

#include "Common/lib/event.h"

TEST(CompletorTest, EndsOnFailure) {
    Sync<std::string> sync{};
    Event event{ Event::Mode::manualReset, Event::State::nonSignalled };

    bool completeFuncCalled{ false };
    auto completeFunc = [&](std::string&) -> bool {
        EXPECT_FALSE(completeFuncCalled);
        completeFuncCalled = true;
        event.Signal();
        throw std::runtime_error{ "Simulated failure from completeFunc" };
    };

    auto jobMatchFunc = [&](const std::string&, const std::string&) {
        ADD_FAILURE() << "Unexpected call to jobMatchFunc";
        return true;
    };

    Completor<std::string> completor{sync, completeFunc, jobMatchFunc};
    event.Wait();
    EXPECT_TRUE(completeFuncCalled);
}
