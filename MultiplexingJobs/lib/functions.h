// Copyright 2021 Mark Bond

#pragma once

#include <functional>

template<typename JobData, typename JobId>
struct Functions {
    using InitiateFunction = std::function<void(const JobData &)>;
    using CompleteFunction = std::function<bool(JobData &, JobId &)>;
    using JobMatchFunction =
        std::function<bool(const JobData &, const JobId &)>;
};
