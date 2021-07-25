// Copyright 2021 Mark Bond

#pragma once

#include <algorithm>
#include <iterator>
#include <list>

#include "lib/state.h"

template<typename JobData>
class Pool {
 public:
    struct ContainerItem {
        ContainerItem(const JobData & jd, const State s) :
            jobData{ jd },
            jobState{ s } {
        }
        JobData jobData{};
        State jobState{};
    };

    using Container = std::list<ContainerItem>;
    using ContainerIt = typename Container::iterator;

 private:
    Container container_{};
    ContainerIt nextToStart_{ container_.begin() };

 public:
    ContainerIt add(const JobData & jobData) {
        container_.emplace_back(jobData, State::initial);
        return std::prev(container_.end());
    }

    bool availableToStart() const {
        return container_.end() != nextToStart_;
    }

    JobData & nextToStart() {
        if ( !availableToStart() ) {
            throw std::runtime_error{ "No job avialable to start" };
        }

        return (nextToStart_++)->jobData;
    }

    template<typename Pred>
    JobData & find_if(Pred pred) {
        const auto it { std::find_if(
            container_.begin(), container_.end(), pred) };
        if ( container_.end() == it ) {
            throw std::runtime_error{ "Job not found in pool" };
        }
        return it->jobData;
    }

    void erase(ContainerIt ref) {
        container_.erase(ref);
    }
};
