#include <cstdlib>
#include <iostream>
#include <locale>

namespace
{
    constexpr auto dotted_lowercase{ 'i' };
    // constexpr auto dotted_uppercase{ '\xc4\xb0' };
    // constexpr auto dotless_lowercase{ '\xc4\xb1' };
    constexpr auto dotless_uppercase{ 'I' };

    constexpr auto dotted_lowercase_w{ L'i' };
    constexpr auto dotted_uppercase_w{ L'\u0130' };
    constexpr auto dotless_lowercase_w{ L'\u0131' };
    constexpr auto dotless_uppercase_w{ L'I' };
}

int main()
{
    try
    {
        const auto from{ dotless_lowercase_w };
        const auto to{ std::toupper(from, std::locale("tr_TR.utf8")) };
        if (dotless_uppercase_w != to)
        {
            throw std::runtime_error{ "Unexpected result" };
        }

        std::cout << "Success" << std::endl;
        return EXIT_SUCCESS;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    std::cout << "Failure" << std::endl;
    return EXIT_FAILURE;
}
