#pragma once

#ifdef __linux__
    #define OS_TEXT(text) text
    #define os_string std::string
    #define os_ostringstream std::ostringstream
#elif defined(_WIN32)
    #define OS_TEXT(text) L##text
    #define os_string std::wstring
    #define os_ostringstream std::wostringstream
#else
    static_assert(false, "Unrecognised OS");
#endif
