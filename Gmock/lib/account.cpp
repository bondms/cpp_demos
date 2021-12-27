// Copyright 2021 Mark Bond

#include <stdexcept>

#include "Gmock/lib/account.h"

Account::Account(Notifier_Interface &notifier) : notifier_{notifier} {}

unsigned int Account::balance() const { return balance_; }

void Account::deposit(unsigned int amount) {
  balance_ += amount;

  notifier_.deposit_made(amount);
}

void Account::withdraw(unsigned int amount) {
  if (amount > balance_) {
    throw std::runtime_error("Balance too small for withdrawal");
  }

  balance_ -= amount;

  notifier_.withdrawal_made(amount);
}
