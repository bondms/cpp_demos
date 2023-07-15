// Copyright 2023 Mark Bond

#pragma once

#include <coroutine>

struct Coro
{
  struct promise_type
  {
    std::suspend_always initial_suspend() noexcept {
      return {};
    }

    std::suspend_always final_suspend() noexcept {
      return {};
    }

    Coro get_return_object() {
      return std::coroutine_handle<promise_type>::from_promise(*this);
    }

    void return_void() {}

    void unhandled_exception() {
      std::terminate();
    }
  };

  Coro(std::coroutine_handle<promise_type> handle): handle_{ handle } {}

  std::coroutine_handle<promise_type> handle_;
};
