#include <cstdlib>
#include <iostream>
#include <locale>

int main()
{
    try
    {
        const auto from{ 'i' };
        std::cout << "From: " << from << std::endl;

        const auto to{ std::toupper(from, std::locale("tr_TR.utf8")) };
        std::cout << "To: " << to << std::endl;

        return EXIT_SUCCESS;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return EXIT_FAILURE;
}
