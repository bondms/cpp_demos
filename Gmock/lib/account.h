// Copyright 2021 Mark Bond

#pragma once

#include "notifier_interface.h"

class Account
{
public:
    explicit Account(Notifier_Interface & notifier);

    Account(const Account &) = delete;
    Account & operator=(const Account &) = delete;

public:
    unsigned long balance() const;

    void deposit(unsigned long amount);
    void withdraw(unsigned long amount);

private:
    unsigned long balance_{0ul};

    Notifier_Interface & notifier_;
};
