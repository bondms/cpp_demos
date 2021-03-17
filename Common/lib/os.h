#pragma once

#ifdef __linux__
    #define OS_TEXT(text) text
    #define os_string string
    #define os_ostringstream ostringstream
#elif defined(_WIN32)
    #define OS_TEXT(text) L##text
    #define os_string wstring
    #define os_ostringstream wostringstream
#else
    static_assert(false, "Unrecognised OS");
#endif
