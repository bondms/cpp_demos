#pragma once

#include <chrono>

template<typename Clock>
class FlushController
{
    const Clock::duration duration_;
    Clock::time_point due_;
public:
    FlushController(const Clock::duration& duration) :
        duration_{duration},
        due_{Clock::now() + duration}
    {
    }

    bool is_due()
    {
        auto now{Clock::now()};

        if(now <= due_)
        {
            return false;
        }

        due_ = now + duration_;
        return true;
    }
};
