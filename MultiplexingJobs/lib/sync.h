// Copyright 2021 Mark Bond

#pragma once

#include <condition_variable>
#include <mutex>
#include <string>

#include "MultiplexingJobs/lib/pool.h"

template <typename JobData> struct Sync {
  std::mutex mutex{};
  std::condition_variable condition_variable{};
  Pool<JobData> pool{};
  bool quit{false};
  std::string error{};
};
