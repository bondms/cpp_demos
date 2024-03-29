// Copyright 2021 Mark Bond

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

#include "Gmock/lib/account.h"
#include "Gmock/lib/notifier.h"

int main() {
  try {
    Notifier notifier{std::cout};
    Account account{notifier};

    while (true) {
      int amount{};
      std::cin >> amount;

      if (0 == amount) {
        break;
      }

      if (amount < 0) {
        account.withdraw(static_cast<unsigned int>(std::abs(amount)));
      } else {
        account.deposit(static_cast<unsigned int>(amount));
      }

      std::cout << "Current balance: " << account.balance() << std::endl;
    }

    std::cout << "Final balance: " << account.balance() << std::endl;

    return EXIT_SUCCESS;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return EXIT_FAILURE;
}
