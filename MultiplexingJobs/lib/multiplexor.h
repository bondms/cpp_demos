// Copyright 2021 Mark Bond

#pragma once

#include <chrono>
#include <utility>

#include "lib/completor.h"
#include "lib/functions.h"
#include "lib/initiator.h"
#include "lib/pool.h"
#include "lib/sync.h"

template<typename JobData, typename JobId>
class Multiplexor {
    Sync<JobData> sync_{};

    const std::chrono::milliseconds timeout_;

    Initiator<JobData, JobId> initiator_;
    Completor<JobData, JobId> completor_;

 public:
    Multiplexor(
                typename Functions<JobData, JobId>::InitiateFunction
                    initiateFunction,
                typename Functions<JobData, JobId>::CompleteFunction
                    completeFunction,
                typename Functions<JobData, JobId>::JobMatchFunction
                    jobMatchFunction,
                std::chrono::milliseconds timeout) :
            timeout_{ timeout },
            initiator_{ sync_, initiateFunction },
            completor_{ sync_, completeFunction, jobMatchFunction } {
    }

    ~Multiplexor() {
        {
            std::lock_guard<std::mutex> lock{ sync_.mutex };
            sync_.quit = true;
        }
        sync_.condition_variable.notify_all();
    }

    void sendAndReceive(JobData & jobData) {
        std::unique_lock<std::mutex> lock{ sync_.mutex };

        auto ref{ sync_.pool.add(jobData) };

        auto pred = [&](){
            return
                sync_.quit
                || (!sync_.error.empty())
                || (State::initial != ref->jobstate);
        };

        if ( !sync_.condition_variable.wait_for(lock, timeout_, pred) ) {
            // Timeout; cancel the job.
            ref->jobState = State::cancelled;
            throw std::runtime_error{ "Timeout" };
        }

        if ( sync_.quit ) {
            throw std::runtime_error{ "Quit" };
        }

        if ( !sync_.error.empty() ) {
            throw std::runtime_error{ "Error: " + sync_.error };
        }

        switch ( ref->jobState ) {
        case State::initial:
            throw std::logic_error{ "Unexpected 'initial' job state" };
        case State::finished:
            break;
        case State::cancelled:
            throw std::runtime_error{ "Cancelled" };
        }

        jobData = std::move(ref->jobData);
        sync_.pool.erase(ref);
    }
};
