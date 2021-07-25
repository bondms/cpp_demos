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
                std::unique_lock<std::mutex> lock{ sync_.mutex };
                sync_.condition_variable.wait(lock, [&]() {
                    return
                        sync_.pool.availableToStart()
                        || sync_.quit
                        || !sync_.error.empty();
                });

                if ( sync_.quit ||  !sync_.error.empty() ) {
                    return;
                }
                initiateFunction_(sync_.pool.nextToStart());
            }
        }
        catch ( const std::exception & e ) {
            std::lock_guard<std::mutex> lock{ sync_.mutex };
            sync_.error = "Initiator exception: "s + e.what();
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
