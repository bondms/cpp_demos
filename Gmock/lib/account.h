// Copyright 2021 Mark Bond

#pragma once

#include "Gmock/lib/notifier_interface.h"

class Account {
public:
  explicit Account(Notifier_Interface &notifier);

  Account(const Account &) = delete;
  Account &operator=(const Account &) = delete;

public:
  unsigned int balance() const;

  void deposit(unsigned int amount);
  void withdraw(unsigned int amount);

private:
  unsigned int balance_{};

  Notifier_Interface &notifier_;
};
