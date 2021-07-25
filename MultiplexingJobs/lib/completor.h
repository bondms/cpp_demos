// Copyright 2021 Mark Bond

#pragma once

#include <thread>
#include <utility>

#include "lib/functions.h"
#include "lib/sync.h"

using std::string_literals::operator""s;

template<typename JobData, typename JobId>
class Completor {
    Sync<JobData> & sync_;
    typename Functions<JobData, JobId>::CompleteFunction completeFunction_;
    typename Functions<JobData, JobId>::JobMatchFunction jobMatchFunction_;

    std::thread thread_{};

    void threadFunc() noexcept {
        try {
            while ( true ) {
                JobData jobData{};
                JobId jobId{};

                const auto completed{ completeFunction_(jobData, jobId) };

                {
                    std::lock_guard<std::mutex> lock{ sync_.mutex };

                    if ( sync_.quit || !sync_.error.empty() ) {
                        return;
                    }

                    if ( completed ) {
                        auto & containerItemRef{
                            sync_.pool.find_if([&](
                                    const typename Pool<JobData>::ContainerItem
                                        & containerItem) {
                                return jobMatchFunction_(
                                    containerItem.jobData, jobId);
                            })
                        };
                        containerItemRef.jobData = std::move(jobData);
                        containerItemRef.jobState = State::finished;
                    }
                }

                if ( completed ) {
                    sync_.condition_variable.notify_all();
                }
            }
        }
        catch ( const std::exception & e ) {
            {
                std::lock_guard<std::mutex> lock{ sync_.mutex };
                sync_.error = "Completor exception: "s + e.what();
            }
            sync_.condition_variable.notify_all();
        }
    }

 public:
    Completor(
                Sync<JobData> & sync,
                typename Functions<JobData, JobId>::CompleteFunction
                    completeFunction,
                typename Functions<JobData, JobId>::JobMatchFunction
                    jobMatchFunction) :
            sync_{ sync },
            completeFunction_{ completeFunction },
            jobMatchFunction_{ jobMatchFunction } {
        thread_ = std::thread{ &Completor::threadFunc, this };
    }

    ~Completor() {
        thread_.join();
    }
};
