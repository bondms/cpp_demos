// Copyright 2021 Mark Bond

#include <gmock/gmock.h>

#include <string>

#include "lib/event.h"
#include "lib/multiplexor.h"

using std::chrono::operator""h;
using std::chrono::operator""s;

TEST(MultiplexorTest, ConstructionAndDestruction) {
    auto nopInitiate = [](const std::string &) { };
    auto nopComplete = [](std::string &, int &) { return true; };
    auto nopJobMatch = [](const std::string &, int) { return true; };

    Multiplexor<std::string, int> jobMultiplexor{
        nopInitiate, nopComplete, nopJobMatch, 1s };
}

TEST(MultiplexorTest, DISABLED_SingleJob) {
    Event event{ Event::Mode::manualReset, Event::State::nonSignalled };

    bool initiateCalled{ false };
    auto initiate = [&](const std::string & jd) {
        EXPECT_FALSE(initiateCalled);
        initiateCalled = true;
        EXPECT_EQ("MyJobInput", jd);
     };

    bool completeCalled{ false };
    auto complete = [&](std::string & jd, int & id) {
        EXPECT_FALSE(completeCalled);
        completeCalled = true;
        jd = "MyJobOutput";
        id = 1;
        event.Signal();
        return true;
    };

    bool jobMatchCalled{ false };
    auto jobMatch = [&](const std::string & jd, int id) {
        EXPECT_FALSE(jobMatchCalled);
        jobMatchCalled = true;
        EXPECT_EQ("MyJobOutput", jd);
        EXPECT_EQ(1, id);
        return true;
    };

    Multiplexor<std::string, int> jobMultiplexor{
        initiate, complete, jobMatch, 1h };

    std::string jobData{ "MyJobInput" };
    jobMultiplexor.sendAndReceive(jobData);
    EXPECT_EQ("MyJobOutput", jobData);
    event.Wait();

    EXPECT_TRUE(initiateCalled);
    EXPECT_TRUE(completeCalled);
    EXPECT_TRUE(jobMatchCalled);
}
