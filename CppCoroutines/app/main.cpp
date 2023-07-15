// Copyright 2023 Mark Bond

#include <chrono>
#include <cstdlib>
#include <iostream>

#include "CppCoroutines/lib/task.h"

using std::chrono_literals::operator""s;

int main() {
  std::cout << "Starting..." << std::endl;

  try {
    // TODO(MarkBond): ...

    std::cout << "Clean shutdown." << std::endl;

    return EXIT_SUCCESS;
  } catch (const std::exception &e) {
    std::cerr << "Main exception: " << e.what() << std::endl;
  }

  return EXIT_FAILURE;
}
