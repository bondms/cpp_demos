// Copyright 2021 Mark Bond

#pragma once

#include <chrono>
#include <string>

namespace SharedPtr {

std::string SlowNoninterruptableGet();

std::string BadGetWithTimeout(std::chrono::milliseconds timeout);
std::string GoodGetWithTimeout(std::chrono::milliseconds timeout);

}  // namespace SharedPtr
