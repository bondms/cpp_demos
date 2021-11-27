// Copyright 2021 Mark Bond

#pragma once

#include <chrono>
#include <utility>

#include "MultiplexingJobs/lib/completor.h"
#include "MultiplexingJobs/lib/functions.h"
#include "MultiplexingJobs/lib/initiator.h"
#include "MultiplexingJobs/lib/pool.h"
#include "MultiplexingJobs/lib/sync.h"

template<typename JobData>
class Multiplexor {
    Sync<JobData> sync_{};

    const std::chrono::milliseconds timeout_;

    Initiator<JobData> initiator_;
    Completor<JobData> completor_;

 public:
    Multiplexor(
                typename Functions<JobData>::InitiateFunction initiateFunction,
                typename Functions<JobData>::CompleteFunction completeFunction,
                typename Functions<JobData>::JobMatchFunction jobMatchFunction,
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

    void processJob(JobData & jobData) {
        typename Pool<JobData>::ContainerIt ref{};

        {
            std::unique_lock<std::mutex> lock{ sync_.mutex };
            ref = sync_.pool.add(jobData);
        }

        sync_.condition_variable.notify_all();

        {
            std::unique_lock<std::mutex> lock{ sync_.mutex };

            auto pred = [&](){
                return
                    sync_.quit
                    || (!sync_.error.empty())
                    || (State::initial != ref->state);
            };

            if ( !sync_.condition_variable.wait_for(lock, timeout_, pred) ) {
                // Timeout; cancel the job.
                // TODO(MarkBond): Something needs to eventually erase the
                // cancelled jobs.
                // Remember, even the completor may never receive a completion.
                ref->state = State::cancelled;
                throw std::runtime_error{ "Timeout" };
            }

            if ( sync_.quit ) {
                throw std::runtime_error{ "Quit" };
            }

            if ( !sync_.error.empty() ) {
                throw std::runtime_error{ "Error: " + sync_.error };
            }

            switch ( ref->state ) {
            case State::finished:
                break;
            case State::cancelled:
                // TODO(MarkBond): Think about this.
                throw std::runtime_error{ "Cancelled" };
            case State::initial:
                throw std::logic_error{ "Unexpected 'initial' job state" };
            }

            jobData = std::move(ref->data);
            sync_.pool.erase(ref);
        }
    }
};
