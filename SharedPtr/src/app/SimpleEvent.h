#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>

class SimpleEvent
{
    std::mutex mutex_{};
    std::condition_variable condition_variable_{};
    bool signalled_{ false };

public:
    void Signal();

    void Wait();

    template<class Rep, class Period>
    bool WaitFor(const std::chrono::duration<Rep, Period> & relTime)
    {
        std::unique_lock<std::mutex> lock{ mutex_ };
        return condition_variable_.wait_for(lock, relTime, [&]{ return signalled_; });
    }

    template<class Clock, class Duration>
    bool WaitUntil(const std::chrono::time_point<Clock, Duration> & timeoutTime)
    {
        std::unique_lock<std::mutex> lock{ mutex_ };
        return condition_variable_.wait_until(lock, timeoutTime, [&]{ return signalled_; });
    }
};
