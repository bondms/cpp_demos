#include "SimpleEvent.h"

void SimpleEvent::Signal()
{
    {
        std::lock_guard<std::mutex> lock{ mutex_ };
        signalled_ = true;
    }
    condition_variable_.notify_all();
}

void SimpleEvent::Wait()
{
    std::unique_lock<std::mutex> lock{ mutex_ };
    condition_variable_.wait(lock, [&]{ return signalled_; });
}
