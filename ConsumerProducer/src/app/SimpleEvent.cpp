#include "SimpleEvent.h"

namespace ConsumerProducer
{
    void SimpleEvent::Set()
    {
        {
            std::lock_guard<std::mutex> lock{ mutex_ };
            set_ = true;
        }
        condition_variable_.notify_one();
    }

    void SimpleEvent::Wait()
    {
        std::unique_lock<std::mutex> lock{ mutex_ };
        condition_variable_.wait(lock, [&]{ return set_; });
    }
} //namespace ConsumerProducer
