#include "FlushController.h"

FlushController::FlushController(const std::chrono::steady_clock::duration& duration) :
    duration_{duration},
    due_{std::chrono::steady_clock::now() + duration}
{
}

bool FlushController::is_due()
{
    auto now{std::chrono::steady_clock::now()};

    if(now <= due_)
    {
        return false;
    }

    due_ = now + duration_;
    return true;
}
