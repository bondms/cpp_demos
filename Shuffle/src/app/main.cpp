#include "shuffle.h"

#include <cstdlib>
#include <iostream>

namespace
{
    template<typename Shuffler>
    void f()
    {
        Shuffler shuffler(9);
        for(auto i = 0 ; i < 25 ; ++i)
        {
            if ( 0 == i % 10 )
            {
                std::cout << '\n';
            }
            std::cout << shuffler() << ", ";
        }
        std::cout << "\n\n";
    }
} // namespace

int main()
{
    std::cout << "Simple: ";
    f<Shuffle::Simple<int>>();

    std::cout << "LowMem: ";
    f<Shuffle::LowMem<int>>();

    return EXIT_SUCCESS;
}
