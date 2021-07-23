// Copyright 2021 Mark Bond

#pragma once

#include "notifier_interface.h"

#include <ostream>

class Notifier :
    public Notifier_Interface
{
public:
    explicit Notifier(std::ostream & os);

public:
    void deposit_made(unsigned long amount) override;
    void withdrawal_made(unsigned long amount) override;

private:
    std::ostream & os_;
};
