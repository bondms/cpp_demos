// Copyright 2021 Mark Bond

#pragma once

#include <chrono>
#include <condition_variable>
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <string>
#include <thread>

template<typename JobData>
class JobMultiplexor {

    class Pool {
     public:
        enum class JobState {
            initial,
            finished,
            cancelled,
        };
        struct ContainerItem {
            JobData jobData{};
            JobState jobState{};
        };
        using Container = std::list<ContainerItem>;
        using ContainerItemRef = Container::iterator;

     private:
        Container container_{};
        ContainerItemRef nextToStart_{ container_.begin() };

     public:
        ContainerItemRef add(const JobData & jobData) {
            container_.emplace_back(jobData, JobState::initial);
        }

        bool availableToStart() const {
            return container_.end() != nextToStart_;
        }

        std::optional<ContainerItemRef> nextToStart() {
            std::optional<ContainerItemRef> result{};
            if ( availableToStart() ) {
                result.jobData = nextToStart_;
                ++nextToStart_;
            }
            return result;
        }
    };

 public:
    using InitiateFunction = std::function<void(const JobData &)>;
    using CompleteFunction = std::function<bool(JobData &)>;

 private:
    std::mutex mutex_{};
    std::condition_variable condition_variable_{};
    Pool pool_{};
    bool quit_{ false };
    std::string error_{};

    class Initiator {
        InitiateFunction initiateFunction_{};
        std::thread thread_{};

        void threadFunc() noexcept {
            while ( true ) {
                std::unique_lock<std::mutex> lock{ mutex_ };
                condition_variable_.wait(lock, []() {
                    return
                        pool_.availableToStart()
                        || quit_
                        || !error_.empty();
                });
                initiateFunction_(jobData);
            }
        }

     public:
        Initiator(InitiateFunction initiateFunction) :
                initiateFunction_{ initiateFunction } {
            thread_ = std::thread{ threadFunc, this }
        }
    };

    class Completor {
        std::thread thread_;
    };

    Initiator initiator_{};
    Completor completor_{};

 public:
    JobMultiplexor(
            InitiateFunction initiateFunction,
            CompleteFunction completeFunction,
            std::chrono::milliseconds timeout) {
        xxx;
    }
};
