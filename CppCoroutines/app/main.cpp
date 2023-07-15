// Copyright 2023 Mark Bond

#include <chrono>
#include <coroutine>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "CppCoroutines/lib/task.h"

using std::chrono_literals::operator""s;

namespace {
void cpu_work() {
  std::cout << "Starting CPU work\n";
  std::this_thread::sleep_for(5s);
  std::cout << "Completed CPU work\n";
}

Coro gpu_work() {
  std::cout << "Starting GPU work\n";
  auto startTime = std::chrono::steady_clock::now();
  while (std::chrono::steady_clock::now() < startTime + 5s) {
    co_await std::suspend_always{};
  }
  std::cout << "Completed GPU work\n";
}
} // namespace

int main() {
  std::cout << "Starting..." << std::endl;

  try {
    auto coro = gpu_work();
    cpu_work();

    while (!coro.done()) {
      coro.resume();
    }

    std::cout << "Clean shutdown." << std::endl;

    return EXIT_SUCCESS;
  } catch (const std::exception &e) {
    std::cerr << "Main exception: " << e.what() << std::endl;
  }

  return EXIT_FAILURE;
}
