// Copyright 2021 Mark Bond

#pragma once

#include <condition_variable>
#include <mutex>
#include <string>

#include "lib/pool.h"

template<typename JobData>
struct Sync {
    std::mutex mutex_{};
    std::condition_variable condition_variable_{};
    Pool<JobData> pool_{};
    bool quit_{ false };
    std::string error_{};
};
