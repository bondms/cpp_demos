// Copyright 2021 Mark Bond

#include "SharedPtr/lib/shared_ptr.h"

#include <memory>
#include <thread>
#include <utility>

#include "Common/lib/event.h"

using std::chrono_literals::operator""s;

namespace SharedPtr {

namespace {

struct Data {
  std::mutex mutex_{};
  std::string result_{};

  Event event_{Event::Mode::manualReset, Event::State::nonSignalled};
};
using DataPtr = std::shared_ptr<Data>;

} // namespace

std::string SlowNoninterruptableGet() {
  std::this_thread::sleep_for(1s);
  return "The data";
}

std::string BadGetWithTimeout(std::chrono::milliseconds timeout) {
  std::string result{};
  Event event{Event::Mode::manualReset, Event::State::nonSignalled};

  std::thread thread{[&] {
    result = SlowNoninterruptableGet();
    event.Signal();
  }};

  event.WaitFor(timeout);
  thread.detach();

  return result;
}

std::string GoodGetWithTimeout(std::chrono::milliseconds timeout) {
  auto dataPtr{std::make_shared<Data>()};

  std::thread thread{[dataPtr] {
    auto localResult{SlowNoninterruptableGet()};
    {
      std::lock_guard<std::mutex> lock{dataPtr->mutex_};
      dataPtr->result_ = std::move(localResult);
    }
    dataPtr->event_.Signal();
  }};

  dataPtr->event_.WaitFor(timeout);
  thread.detach();

  {
    std::lock_guard<std::mutex> lock{dataPtr->mutex_};
    return std::move(dataPtr->result_);
  }
}

} // namespace SharedPtr
