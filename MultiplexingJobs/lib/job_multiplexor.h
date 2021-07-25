// Copyright 2021 Mark Bond

#pragma once

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <list>
#include <mutex>
#include <string>
#include <thread>

using std::string_literals::operator""s;

template<typename JobData, typename JobId>
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
        using ContainerItemRef = typename Container::iterator;

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

        ContainerItemRef nextToStart() {
            if ( !availableToStart() ) {
                throw std::runtime_error{ "No job avialable to start" };
            }

            return nextToStart_++;
        }

        template<typename Pred>
        ContainerItemRef find_if(Pred pred) {
            const auto it { std::find_if(
                container_.begin(), container_.end(), pred) };
            if ( container_.end() == it ) {
                throw std::runtime_error{ "Job not found in pool" };
            }
            return it;
        }

        void erase(ContainerItemRef ref) {
            container_.erase(ref);
        }
    };

 public:
    using InitiateFunction = std::function<void(const JobData &)>;
    using CompleteFunction = std::function<bool(JobData &, JobId &)>;
    using JobMatchFunction =
        std::function<bool(const JobData &, const JobId &)>;

 private:
    std::mutex mutex_{};
    std::condition_variable condition_variable_{};
    Pool pool_{};
    bool quit_{ false };
    std::string error_{};

    const std::chrono::milliseconds timeout_;

    class Initiator {
        JobMultiplexor<JobData, JobId> & jobMultiplexor_;
        InitiateFunction initiateFunction_{};

        std::thread thread_{};

        void threadFunc() noexcept {
            try {
                while ( true ) {
                    std::unique_lock<std::mutex> lock{ jobMultiplexor_.mutex_ };
                    jobMultiplexor_.condition_variable_.wait(lock, [&]() {
                        return
                            jobMultiplexor_.pool_.availableToStart()
                            || jobMultiplexor_.quit_
                            || !jobMultiplexor_.error_.empty();
                    });

                    if ( jobMultiplexor_.quit_
                            ||  !jobMultiplexor_.error_.empty() ) {
                        return;
                    }
                    initiateFunction_(
                        jobMultiplexor_.pool_.nextToStart()->jobData);
                }
            }
            catch ( const std::exception & e ) {
                std::lock_guard<std::mutex> lock{ jobMultiplexor_.mutex_ };
                jobMultiplexor_.error_ = "Initiator exception: "s + e.what();
            }
        }

     public:
        Initiator(
                    JobMultiplexor<JobData, JobId> & jobMultiplexor,
                    InitiateFunction initiateFunction) :
                jobMultiplexor_{ jobMultiplexor },
                initiateFunction_{ initiateFunction } {
            thread_ = std::thread{ &Initiator::threadFunc, this };
        }

        ~Initiator() {
            thread_.join();
        }
    };

    class Completor {
        JobMultiplexor<JobData, JobId> & jobMultiplexor_;
        CompleteFunction completeFunction_;
        JobMatchFunction jobMatchFunction_;

        std::thread thread_{};

        void threadFunc() noexcept {
            try {
                while ( true ) {
                    JobData jobData{};
                    JobId jobId{};

                    const auto completed{ completeFunction_(jobData, jobId) };

                    std::lock_guard<std::mutex> lock{ jobMultiplexor_.mutex_ };

                    if ( jobMultiplexor_.quit_
                         || !jobMultiplexor_.error_.empty() ) {
                        return;
                    }

                    if ( completed ) {
                        auto containerItemRef{
                            jobMultiplexor_.pool_.find_if([&](
                                    const typename Pool::ContainerItem & containerItem) {
                                return jobMatchFunction_(
                                    containerItem.jobData, jobId);
                            })
                        };
                        containerItemRef->jobData = std::move(jobData);
                    }
                }
            }
            catch ( const std::exception & e ) {
                std::lock_guard<std::mutex> lock{ jobMultiplexor_.mutex_ };
                jobMultiplexor_.error_ = "Completor exception: "s + e.what();
            }
        }

     public:
        Completor(
                    JobMultiplexor<JobData, JobId> & jobMultiplexor,
                    CompleteFunction completeFunction,
                    JobMatchFunction jobMatchFunction) :
                jobMultiplexor_{ jobMultiplexor },
                completeFunction_{ completeFunction },
                jobMatchFunction_{ jobMatchFunction } {
            thread_ = std::thread{ &Completor::threadFunc, this };
        }

        ~Completor() {
            thread_.join();
        }
    };

    Initiator initiator_;
    Completor completor_;

 public:
    JobMultiplexor(
                InitiateFunction initiateFunction,
                CompleteFunction completeFunction,
                JobMatchFunction jobMatchFunction,
                std::chrono::milliseconds timeout) :
            timeout_{ timeout },
            initiator_{ *this, initiateFunction },
            completor_{ *this, completeFunction, jobMatchFunction } {
    }

    ~JobMultiplexor() {
        std::lock_guard<std::mutex> lock{ mutex_ };
        quit_ = true;
    }

    void sendAndReceive(JobData & jobData) {
        std::unique_lock<std::mutex> lock{ mutex_ };

        auto ref{ pool_.add(jobData) };

        if ( ! condition_variable_.wait_for(lock, timeout_, [&](){
            if ( quit_ || !error_.empty() ) {
                return true;
            }
            switch ( ref->jobState ) {
            case Pool::JobState::finished:
            case Pool::JobState::cancelled:
                return true;
            }
            return false;
        }) ) {
            // Timeout; cancel the job.
            ref->jobState = Pool::JobState::cancelled;
            throw std::runtime_error{ "Timeout" };
        }

        if ( quit_ ) {
            throw std::runtime_error{ "Quit" };
        }

        if ( !error_.empty() ) {
            throw std::runtime_error{ "Error: " + error_ };
        }

        switch ( ref->jobState ) {
        case Pool::JobState::initial:
            throw std::logic_error{ "Unexpected 'initial' job state" };
        case Pool::JobState::finished:
            break;
        case Pool::JobState::cancelled:
            throw std::runtime_error{ "Cancelled" };
        }

        jobData = std::move(ref->jobData);
        pool_.erase(ref);
    }
};
