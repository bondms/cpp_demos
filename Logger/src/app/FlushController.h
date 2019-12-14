#pragma once

#include <chrono>

template<typename Clock>
class FlushController
{
    const typename Clock::duration duration_;
    typename Clock::time_point due_;
public:
    FlushController(const typename Clock::duration& duration) :
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
