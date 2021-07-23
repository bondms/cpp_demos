// Copyright 2021 Mark Bond

#include "lib/account.h"
#include "lib/notifier.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

int main()
{
    try
    {
        Notifier notifier{std::cout};
        Account account{notifier};

        while ( true )
        {
            long amount{};
            std::cin >> amount;

            if ( 0 == amount )
            {
                break;
            }

            if ( amount < 0 )
            {
                account.withdraw(std::abs(amount));
            }
            else
            {
                account.deposit(amount);
            }

            std::cout << "Current balance: " << account.balance() << std::endl;
        }

        std::cout << "Final balance: " << account.balance() << std::endl;

        return EXIT_SUCCESS;
    }
    catch ( const std::exception & e )
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return EXIT_FAILURE;
}
