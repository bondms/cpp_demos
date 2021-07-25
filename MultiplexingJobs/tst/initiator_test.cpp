// Copyright 2021 Mark Bond

#include <gmock/gmock.h>

#include "lib/initiator.h"

TEST(InitiatorTest, NoJobs) {
    Sync<std::string> sync{};

    // auto initiateFunc = [&](const std::string&) -> void {
    //     throw std::runtime_error{ "Unexpected call to initiatFunc" };
    // };

    // Initiator<std::string, int> initiator{sync, initiateFunc};
}
