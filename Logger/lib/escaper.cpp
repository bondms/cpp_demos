// Copyright 2021 Mark Bond

#include "lib/escaper.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

namespace Escaper
{
    namespace
    {
        // Similar to std::isprint, except:
        // * Always uses "C" locale, regardless of the currently installed C locale.
        // * Thread safe with regard to concurrent std::setlocale calls.
        // * Doesn't require casting to unsigned char, but doesn't accept EOF.
        bool isprint(char ch)
        {
            return ch >= 0x20 && ch <= 0x7E;
        }

        bool NeedsEscaping(char ch)
        {
            if ( '\\' == ch ) return true;
            return !isprint(ch);
        }

        // Returns an escaped/printable version of the raw character WITHOUT a leading '\'
        std::string Escaped(char raw)
        {
            switch (raw)
            {
                case '\0': return R"<<<(0)<<<";
                case '\a': return R"<<<(a)<<<";
                case '\b': return R"<<<(b)<<<";
                case '\f': return R"<<<(f)<<<";
                case '\n': return R"<<<(n)<<<";
                case '\r': return R"<<<(r)<<<";
                case '\t': return R"<<<(t)<<<";
                case '\v': return R"<<<(v)<<<";
                case '\\': return R"<<<(\)<<<";
            }

            std::ostringstream oss{};
            oss
                << "x"
                << std::hex << std::setfill('0') << std::setw(2) << std::uppercase
                << static_cast<unsigned int>(static_cast<unsigned char>(raw));
            return oss.str();
        }
    } // namespace

    std::string Escaped(const std::string & raw)
    {
        std::string result = raw;

        auto it = result.begin();
        while ( true )
        {
            // Find next character that needs escaping.
            it = std::find_if(it, result.end(), NeedsEscaping );
            if ( it == result.end() )
            {
                break;
            }

            auto escaped = Escaped(*it);

            // Replace the non-printable character with a '\' and insert the escaped sequence following the '\'.
            *it = '\\';
            ++it;
            it = result.insert(it, escaped.begin(), escaped.end());

            // Continue from after the instered escaped sequence.
            it += escaped.size();
        }

        return result;
    }
} // Escaper
