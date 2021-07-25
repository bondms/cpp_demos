// Copyright 2021 Mark Bond

#include <gmock/gmock.h>

#include "lib/multiplexor.h"

using std::chrono::operator""s;

TEST(MultiplexorTest, ConstructionAndDestruction) {
    struct Data {
        std::string str;
    };
    auto nopInitiate = [](const Data &) { };
    auto nopComplete = [](Data &, int &) { return true; };
    auto nopJobMatch = [](const Data &, int) { return true; };
    Multiplexor<Data, int> jobMultiplexor{
        nopInitiate, nopComplete, nopJobMatch, 1s };
}

// TODO(MarkBond): Implement more tests.
