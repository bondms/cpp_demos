// Copyright 2021 Mark Bond

#include <gmock/gmock.h>

#include <stdexcept>
#include <string>

#include "lib/job_multiplexor.h"

using std::chrono::operator""s;

TEST(JobMultiplexorTest, ConstructionAndDestruction) {
    struct Data {
        std::string str;
    };
    auto nopInitiate = [](const Data &) { };
    auto nopComplete = [](Data &, int &) { return true; };
    auto nopJobMatch = [](const Data &, int) { return true; };
    JobMultiplexor<Data, int> jobMultiplexor{
        nopInitiate, nopComplete, nopJobMatch, 1s };
}

TEST(JobMultiplexorTest, TODO) {
    ADD_FAILURE() << "TODO(MarkBond): Implement";
}
