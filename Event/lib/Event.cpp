#include "lib/Event.h"

Event::Event(Mode mode, State initialState) :
    mode_{ mode },
    state_{ initialState }
{
}

bool Event::WaitPredicate()
{
    if ( State::nonSignalled == state_ )
    {
        return false;
    }

    if ( Mode::autoReset == mode_ )
    {
        state_ = State::nonSignalled;
    }

    return true;
}

void Event::Signal()
{
    {
        std::lock_guard<std::mutex> lock{ mutex_ };
        state_ = State::signalled;
    }

    if ( Mode::autoReset == mode_ )
    {
        condition_variable_.notify_one();
    }
    else
    {
        condition_variable_.notify_all();
    }
}

void Event::Reset()
{
    std::lock_guard<std::mutex> lock{ mutex_ };
    state_ = State::nonSignalled;
}

void Event::Wait()
{
    std::unique_lock<std::mutex> lock{ mutex_ };
    condition_variable_.wait(lock, [&]{ return WaitPredicate(); });
}
