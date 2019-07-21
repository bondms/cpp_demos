#include "notifier.h"

Notifier::Notifier(std::ostream & os) :
    os_{os}
{
}

void Notifier::deposit_made(unsigned long amount)
{
    os_ << "Deposited: " << amount << std::endl;
}

void Notifier::withdrawal_made(unsigned long amount)
{
    os_ << "Withdrawn: " << amount << std::endl;
}
