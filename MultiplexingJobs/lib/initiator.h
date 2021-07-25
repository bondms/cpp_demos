// Copyright 2021 Mark Bond

#pragma once

#include <thread>

#include "lib/functions.h"
#include "lib/sync.h"

using std::string_literals::operator""s;

template<typename JobData, typename JobId>
class Initiator {
    Sync<JobData> & sync_;
    typename Functions<JobData, JobId>::InitiateFunction initiateFunction_{};

    std::thread thread_{};

    void threadFunc() noexcept {
        try {
            while ( true ) {
                std::unique_lock<std::mutex> lock{ sync_.mutex_ };
                sync_.condition_variable_.wait(lock, [&]() {
                    return
                        sync_.pool_.availableToStart()
                        || sync_.quit_
                        || !sync_.error_.empty();
                });

                if ( sync_.quit_ ||  !sync_.error_.empty() ) {
                    return;
                }
                initiateFunction_(sync_.pool_.nextToStart());
            }
        }
        catch ( const std::exception & e ) {
            std::lock_guard<std::mutex> lock{ sync_.mutex_ };
            sync_.error_ = "Initiator exception: "s + e.what();
        }
    }

 public:
    Initiator(
                Sync<JobData> & sync,
                typename Functions<JobData, JobId>::InitiateFunction
                    initiateFunction) :
            sync_{ sync },
            initiateFunction_{ initiateFunction } {
        thread_ = std::thread{ &Initiator::threadFunc, this };
    }

    ~Initiator() {
        thread_.join();
    }
};
