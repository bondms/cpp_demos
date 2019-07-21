#ifndef __BINARY_SEARCH_H__
#define __BINARY_SEARCH_H__

#include <iterator>

template<typename ElementType, typename InputIt>
InputIt binary_search(InputIt first, InputIt last, const ElementType & item)
{
    while ( true )
    {
        const auto distance{ std::distance(first, last) };

        if ( 0 == distance )
        {
            // Not found.
            // Return first rather than last in case last is sentinal.
            return first;
        }

        const auto mid{ std::next(first, distance / 2) };
        const auto & found{ *mid };

        if ( found < item )
        {
            first = std::next(mid);
        }
        else if ( found > item )
        {
            last = mid;
        }
        else
        {
            // Found.
            return mid;
        }
    }
}

#endif // ndef __BINARY_SEARCH_H__
