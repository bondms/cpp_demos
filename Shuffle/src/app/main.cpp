#include "shuffle.h"

#include <cstdlib>
#include <iostream>

int main()
{
    std::cout << "Simple: ";
    Shuffle::Simple<int> simple(9);
    for(auto i = 0 ; i < 25 ; ++i)
    {
        if (0==i%10)
        {
            std::cout << std::endl;
        }
        std::cout << simple() << ", ";
    }
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
