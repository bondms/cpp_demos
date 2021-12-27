// Copyright 2021 Mark Bond

#pragma once

class Notifier_Interface {
protected:
  virtual ~Notifier_Interface() = default;

public:
  virtual void deposit_made(unsigned int amount) = 0;
  virtual void withdrawal_made(unsigned int amount) = 0;
};
