#include "shuffle.h"

#include <cstdlib>
#include <iostream>

namespace
{
    template<typename Shuffler>
    void f(Shuffler & shuffler)
    {
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
    Shuffle::Simple<int> simple{9};
    f(simple);

    std::cout << "LowMem: ";
    Shuffle::LowMem<int, 9> lowMem{};
    f(lowMem);

    return EXIT_SUCCESS;
}
