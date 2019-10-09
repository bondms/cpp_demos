#pragma once

#include <algorithm>
#include <cctype>

template<typename Container>
void realign(Container & container)
{
    auto r_it_source{ std::find_if_not(container.rbegin(), container.rend(), std::isspace) };
    auto r_it_moved{ std::move(r_it_source, container.rend(), container.rbegin()) };
    std::fill(r_it_moved, container.rend(), static_cast<typename Container::value_type>(' '));
}
