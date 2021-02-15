#include "lib/Event.h"
#include "lib/SharedPtr.h"

#include <thread>
#include <utility>

using namespace std::chrono_literals;

namespace SharedPtr
{
    namespace
    {
        struct Data
        {
            std::mutex mutex_{};
            std::string result_{};

            Event event_{Event::Mode::manualReset, Event::State::nonSignalled};
        };
        using DataPtr = std::shared_ptr<Data>;
    }

    std::string SlowNoninterruptableGet()
    {
        std::this_thread::sleep_for(1s);
        return "The data";
    }

    std::string BadGetWithTimeout(std::chrono::milliseconds timeout)
    {
        std::string result{};
        Event event{Event::Mode::manualReset, Event::State::nonSignalled};

        std::thread thread{[&]{
            result = SlowNoninterruptableGet();
            event.Signal();
        }};

        event.WaitFor(timeout);
        thread.detach();

        return result;
    }

    std::string GoodGetWithTimeout(std::chrono::milliseconds timeout)
    {
        auto dataPtr{ std::make_shared<Data>() };

        std::thread thread{[dataPtr]{
            auto localResult{ SlowNoninterruptableGet() };
            {
                std::lock_guard<std::mutex> lock{ dataPtr->mutex_ };
                dataPtr->result_ = std::move(localResult);
            }
            dataPtr->event_.Signal();
        }};

        dataPtr->event_.WaitFor(timeout);
        thread.detach();

        {
            std::lock_guard<std::mutex> lock{ dataPtr->mutex_ };
            return std::move(dataPtr->result_);
        }
    }
} // namespace SharedPtr
