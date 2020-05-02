#include "Escaper.h"

#include <iomanip>
#include <sstream>

namespace Escaper
{
    namespace
    {
        // Similar to std::isprint, except:
        // * Always uses "C" locale, regardless of the currently installed C locale.
        // * Thread safe with regard to concurrent std::setlocale calls.
        // * Doesn't require casting to unsigned char.
        bool isprint(char ch)
        {
            return ch >= 0x20 && ch <= 0x7E;
        }

        std::string Escaped(char raw)
        {
            if ('\\' == raw)
            {
                return R"<<<(\\)<<<";
            }

            if (isprint(raw))
            {
                return std::string(1, raw);
            }

            switch (raw)
            {
                case '\0': return R"<<<(\0)<<<";
                case '\a': return R"<<<(\a)<<<";
                case '\b': return R"<<<(\b)<<<";
                case '\f': return R"<<<(\f)<<<";
                case '\n': return R"<<<(\n)<<<";
                case '\r': return R"<<<(\r)<<<";
                case '\t': return R"<<<(\t)<<<";
                case '\v': return R"<<<(\v)<<<";
            }

            std::ostringstream oss{};
            oss
                << "\\x"
                << std::hex << std::setfill('0') << std::setw(2) << std::uppercase
                << static_cast<unsigned int>(static_cast<unsigned char>(raw));
            return oss.str();
        }
    } // namespace

    std::string Escaped(const std::string & raw)
    {
        std::string result{};
        for (const auto & ch : raw)
        {
            result += Escaped(ch);
        }
        return result;
    }
} // Escaper
