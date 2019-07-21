#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>

class Event
{
public:
    enum class State
    {
        nonSignalled,
        signalled
    };

    enum class Mode
    {
        manualReset,
        autoReset
    };

private:
    const Mode mode_{};

    std::mutex mutex_{};
    std::condition_variable condition_variable_{};
    State state_{};

    bool WaitPredicate();

public:
    Event(Mode mode, State initialState);

    void Signal();

    void Reset();

    void Wait();

    template<class Rep, class Period>
    bool WaitFor(const std::chrono::duration<Rep, Period> & relTime)
    {
        std::unique_lock<std::mutex> lock{ mutex_ };
        return condition_variable_.wait_for(lock, relTime, [&]{ return WaitPredicate(); });
    }

    template<class Clock, class Duration>
    bool WaitUntil(const std::chrono::time_point<Clock, Duration> & timeoutTime)
    {
        std::unique_lock<std::mutex> lock{ mutex_ };
        return condition_variable_.wait_until(lock, timeoutTime, [&]{ return WaitPredicate(); });
    }
};
