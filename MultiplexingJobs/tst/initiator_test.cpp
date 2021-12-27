// Copyright 2021 Mark Bond

#include <gmock/gmock.h>

#include "MultiplexingJobs/lib/initiator.h"

TEST(InitiatorTest, EndsOnQuit) {
  Sync<std::string> sync{};

  auto initiateFunc = [&](const std::string &) -> void {
    throw std::runtime_error{"Unexpected call to initiatFunc"};
  };

  Initiator<std::string> initiator{sync, initiateFunc};

  {
    std::unique_lock<std::mutex> lock{sync.mutex};
    sync.quit = true;
  }
  sync.condition_variable.notify_one();
}
