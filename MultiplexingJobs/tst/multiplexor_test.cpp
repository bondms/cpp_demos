// Copyright 2021 Mark Bond

#include <gmock/gmock.h>

#include <string>

#include "lib/event.h"
#include "lib/multiplexor.h"

using std::chrono::operator""s;

TEST(MultiplexorTest, ConstructionAndDestruction) {
    auto nopInitiate = [](const std::string &) { };
    auto nopComplete = [](std::string &, int &) { return true; };
    auto nopJobMatch = [](const std::string &, int) { return true; };

    Multiplexor<std::string, int> jobMultiplexor{
        nopInitiate, nopComplete, nopJobMatch, 1s };
}

TEST(MultiplexorTest, SingleJob) {
    Event initiatedEvent{ Event::Mode::manualReset, Event::State::nonSignalled };
    Event completedEvent{ Event::Mode::manualReset, Event::State::nonSignalled };

    bool initiateCalled{ false };
    auto initiate = [&](const std::string & jd) {
        EXPECT_FALSE(initiateCalled);
        initiateCalled = true;
        EXPECT_EQ("MyJobInput", jd);
        initiatedEvent.Signal();
     };

    bool completeCalled{ false };
    auto complete = [&](std::string & jd, int & id) {
        initiatedEvent.Wait();
        if ( completeCalled ) {
            std::this_thread::sleep_for(1s);
            return false;
        }
        completeCalled = true;
        jd = "MyJobOutput";
        id = 1;
        return true;
    };

    bool jobMatchCalled{ false };
    auto jobMatch = [&](const std::string & jd, int id) {
        EXPECT_FALSE(jobMatchCalled);
        jobMatchCalled = true;
        EXPECT_EQ("MyJobInput", jd);
        EXPECT_EQ(1, id);
        completedEvent.Signal();
        return true;
    };

    Multiplexor<std::string, int> jobMultiplexor{
        initiate, complete, jobMatch, 5s };

    std::string jobData{ "MyJobInput" };
    jobMultiplexor.sendAndReceive(jobData);
    EXPECT_EQ("MyJobOutput", jobData);

    completedEvent.Wait();

    EXPECT_TRUE(initiateCalled);
    EXPECT_TRUE(completeCalled);
    EXPECT_TRUE(jobMatchCalled);
}
