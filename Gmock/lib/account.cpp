// Copyright 2021 Mark Bond

#include "lib/account.h"

#include <stdexcept>

Account::Account(Notifier_Interface & notifier) :
    notifier_{notifier}
{
}

unsigned long Account::balance() const
{
    return balance_;
}

void Account::deposit(unsigned long amount)
{
    balance_ += amount;

    notifier_.deposit_made(amount);
}

void Account::withdraw(unsigned long amount)
{
    if ( amount > balance_ )
    {
        throw std::runtime_error("Balance too small for withdrawal");
    }

    balance_ -= amount;

    notifier_.withdrawal_made(amount);
}
