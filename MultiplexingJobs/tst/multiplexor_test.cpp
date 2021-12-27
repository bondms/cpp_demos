// Copyright 2021 Mark Bond

#include <gmock/gmock.h>

#include <string>

#include "MultiplexingJobs/lib/multiplexor.h"

#include "Common/lib/event.h"

using std::chrono::operator""s;

TEST(MultiplexorTest, ConstructionAndDestruction) {
  auto nopInitiate = [](const std::string &) {};
  auto nopComplete = [](std::string &) { return true; };
  auto nopJobMatch = [](const std::string &, const std::string &) {
    return true;
  };

  Multiplexor<std::string> jobMultiplexor{nopInitiate, nopComplete, nopJobMatch,
                                          1s};
}

TEST(MultiplexorTest, SingleJob) {
  Event initiatedEvent{Event::Mode::manualReset, Event::State::nonSignalled};

  bool initiateCalled{false};
  auto initiate = [&](const std::string &jobData) {
    EXPECT_FALSE(initiateCalled);
    initiateCalled = true;
    EXPECT_EQ("MyJobInput", jobData);
    initiatedEvent.Signal();
  };

  bool completeCalled{false};
  auto complete = [&](std::string &jobData) {
    initiatedEvent.Wait();
    if (completeCalled) {
      std::this_thread::sleep_for(1s);
      return false;
    }
    completeCalled = true;
    jobData = "MyJobOutput";
    return true;
  };

  bool jobMatchCalled{false};
  auto jobMatch = [&](const std::string &input, const std::string &output) {
    EXPECT_FALSE(jobMatchCalled);
    jobMatchCalled = true;
    EXPECT_EQ("MyJobInput", input);
    EXPECT_EQ("MyJobOutput", output);
    return true;
  };

  Multiplexor<std::string> jobMultiplexor{initiate, complete, jobMatch, 5s};

  std::string jobData{"MyJobInput"};
  jobMultiplexor.processJob(jobData);
  EXPECT_EQ("MyJobOutput", jobData);

  EXPECT_TRUE(initiateCalled);
  EXPECT_TRUE(completeCalled);
  EXPECT_TRUE(jobMatchCalled);
}
