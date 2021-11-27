// Copyright 2021 Mark Bond

#include "Gmock/lib/notifier.h"

Notifier::Notifier(std::ostream & os) :
    os_{ os } {
}

void Notifier::deposit_made(unsigned int amount) {
    os_ << "Deposited: " << amount << std::endl;
}

void Notifier::withdrawal_made(unsigned int amount) {
    os_ << "Withdrawn: " << amount << std::endl;
}
