// Copyright 2021 Mark Bond

#include <cstdlib>
#include <iostream>

#include "lib/shuffle.h"

namespace
{

template<typename Shuffler>
void f(Shuffler & shuffler) {
    const auto max = shuffler.max();
    for ( auto i = 0 ; i < (max * 2 + max / 2 + 1) ; ++i ) {
        if ( 0 == i % (max + 1) ) {
            std::cout << '\n';
        }
        std::cout << shuffler() << ", ";
    }
    std::cout << "\n\n";
}

}  // namespace

int main() {
    std::cout << "Simple: ";
    Shuffle::Simple<int> simple{9};
    f(simple);

    std::cout << "LowMem: ";
    Shuffle::LowMem<int, 9> lowMem{};
    f(lowMem);

    return EXIT_SUCCESS;
}
