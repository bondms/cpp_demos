Synchronous generators will be trivial without ASIO once `std::generator` is widely available in C++23, for example:

``` c++
#include <coroutine>
#include <experimental/generator>
#include <iostream>
#include <random>

std::experimental::generator<int> randomGenerator(int min, int max)
{
    std::random_device rd{};
    std::default_random_engine re{ rd() };
    std::uniform_int_distribution<int> uid{ min, max };

    while ( true )
    {
        co_yield uid(rd);
    }
}

template<typename G>
std::experimental::generator<int> countedGenerator(G& generator, int count)
{
    auto it = generator.begin();
    for ( auto i = 0 ; i < count ; ++i )
    {
        co_yield *it;
        ++it;
    }
}

template<typename G>
std::experimental::generator<int> oddifier(G& generator)
{
    for ( const auto & i : generator )
    {
        co_yield (i%2==0) ? i+1 : i;
    }
}

int main()
{
    auto rg = randomGenerator(0, 99);
    auto o = oddifier(rg);
    for ( const auto & i :
      countedGenerator<std::experimental::generator<int>>(o, 20) )
    {
        std::cout << i << '\n';
    }
    std::cout << "Done" << std::endl;
}
```
