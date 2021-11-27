// Copyright 2021 Mark Bond

#pragma once

#include <thread>
#include <utility>

#include "MultiplexingJobs/lib/functions.h"
#include "MultiplexingJobs/lib/sync.h"

using std::string_literals::operator""s;

template<typename JobData>
class Completor {
    Sync<JobData> & sync_;
    typename Functions<JobData>::CompleteFunction completeFunction_;
    typename Functions<JobData>::JobMatchFunction jobMatchFunction_;

    std::thread thread_{};

    void threadFunc() noexcept {
        try {
            while ( true ) {
                JobData jobData{};
                const auto completed{ completeFunction_(jobData) };

                {
                    std::lock_guard<std::mutex> lock{ sync_.mutex };

                    if ( sync_.quit || !sync_.error.empty() ) {
                        return;
                    }

                    if ( completed ) {
                        // TODO(MarkBond): Can the item have been erased?
                        auto & containerItemRef{
                            sync_.pool.find_if([&](
                                    const typename Pool<JobData>::ContainerItem
                                        & containerItem) {
                                return jobMatchFunction_(
                                    containerItem.data, jobData);
                            })
                        };
                        containerItemRef.data = std::move(jobData);
                        containerItemRef.state = State::finished;
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
                typename Functions<JobData>::CompleteFunction completeFunction,
                typename Functions<JobData>::JobMatchFunction
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
