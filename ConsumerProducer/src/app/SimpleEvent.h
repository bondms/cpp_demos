#pragma once

#include <condition_variable>
#include <mutex>

namespace ConsumerProducer
{
    class SimpleEvent
    {
        std::mutex mutex_{};
        std::condition_variable condition_variable_{};
        bool set_{ false };

    public:
        void Set();
        void Wait();
    };
} //namespace ConsumerProducer
