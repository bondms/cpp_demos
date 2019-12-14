#pragma once

#include <chrono>

class FlushController
{
    const std::chrono::steady_clock::duration duration_;
    std::chrono::steady_clock::time_point due_;
public:
    FlushController(const std::chrono::steady_clock::duration& duration);
    bool is_due();
};
