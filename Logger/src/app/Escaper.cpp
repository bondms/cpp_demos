#include "Escaper.h"

#include <iomanip>
#include <sstream>

namespace Escaper
{
    namespace
    {
        // Similar to std::iscntrl, except:
        // * Always uses "C" locale, regardless of the currently installed C locale.
        // * Thread safe with regard to concurrent std::setlocale calls.
        // * Doesn't require casting to unsigned char.
        bool iscntrl(char ch)
        {
            return (ch >= 0x00 && ch <= 0x1F) || (ch == 0x7F);
        }

        std::string Escaped(char raw)
        {
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
                case '\\': return R"<<<(\\)<<<";
            }

            if (iscntrl(raw))
            {
                std::ostringstream oss{};
                oss
                    << "\\x"
                    << std::hex << std::setfill('0') << std::setw(2) << std::uppercase << static_cast<unsigned int>(raw);
                return oss.str();
            }

            return std::string(1, raw);
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
