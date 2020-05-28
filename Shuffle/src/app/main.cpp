#include "shuffle.h"

#include <cstdlib>
#include <iostream>

int main()
{
    std::cout << "Simple: ";
    Shuffle::Simple<int> simple(9);
    for(auto i = 0 ; i < 25 ; ++i)
    {
        if ( 0 == i % 10 )
        {
            std::cout << '\n';
        }
        std::cout << simple() << ", ";
    }
    std::cout << "\n\n";

    std::cout << "LowMem: ";
    Shuffle::LowMem<int> lowmem(9);
    for(auto i = 0 ; i < 25 ; ++i)
    {
        if ( 0 == i % 10 )
        {
            std::cout << '\n';
        }
        std::cout << lowmem() << ", ";
    }
    std::cout << "\n\n";

    return EXIT_SUCCESS;
}
