// Copyright 2021 Mark Bond

#pragma once

#include <algorithm>
#include <iterator>
#include <list>

#include "MultiplexingJobs/lib/state.h"

template<typename JobData>
class Pool {
 public:
    struct ContainerItem {
        ContainerItem(const JobData & jobData, const State jobState) :
            data{ jobData },
            state{ jobState } {
        }
        JobData data{};
        State state{};
    };

    using Container = std::list<ContainerItem>;
    using ContainerIt = typename Container::iterator;

 private:
    Container container_{};
    ContainerIt nextToStart_{ container_.begin() };

 public:
    ContainerIt add(const JobData & jobData) {
        container_.emplace_back(jobData, State::initial);
        auto added{ std::prev(container_.end()) };
        if ( container_.end() == nextToStart_ ) {
            nextToStart_ = added;
        }
        return added;
    }

    bool availableToStart() const {
        return container_.end() != nextToStart_;
    }

    JobData & nextToStart() {
        if ( !availableToStart() ) {
            throw std::runtime_error{ "No job avialable to start" };
        }

        return (nextToStart_++)->data;
    }

    template<typename Pred>
    ContainerItem & find_if(Pred pred) {
        const auto it { std::find_if(
            container_.begin(), container_.end(), pred) };
        if ( container_.end() == it ) {
            throw std::runtime_error{ "Job not found in pool" };
        }
        return *it;
    }

    void erase(ContainerIt ref) {
        if ( nextToStart_ == ref ) {
            ++nextToStart_;
        }
        container_.erase(ref);
    }
};
