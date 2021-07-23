// Copyright 2021 Mark Bond
// Based on scope_exit from https://bajamircea.github.io/coding/cpp/2018/04/12/scope-guard.html

#pragma once

#include <type_traits>
#include <utility>

template<typename F>
class ScopeExit {
    F f_;

public:
    explicit ScopeExit(const F& f) :
        f_{f}
    {
    }

    explicit ScopeExit(F&& f) :
        f_{std::move(f)}
    {
    }

    ~ScopeExit() {
        f_();
    }

    ScopeExit(const ScopeExit&) = delete;
    ScopeExit& operator=(const ScopeExit&) = delete;

    ScopeExit(ScopeExit&&) = default;
    ScopeExit& operator=(ScopeExit&&) = default;
};

template<typename F>
auto make_scope_exit(F&& f) {
    return ScopeExit<std::remove_reference_t<F>>(std::forward<F>(f));
}
