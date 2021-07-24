// Copyright 2021 Mark Bond

#pragma once

#include <ostream>

#include "lib/notifier_interface.h"

class Notifier :
    public Notifier_Interface {
 public:
    explicit Notifier(std::ostream & os);

 public:
    void deposit_made(unsigned int amount) override;
    void withdrawal_made(unsigned int amount) override;

 private:
    std::ostream & os_;
};
