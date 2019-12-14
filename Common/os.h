#pragma once

#ifdef __linux__ 
    #define OS_TEXT(text) text
#elif defined(_WIN32)
    #define OS_TEXT(text) L##text
#else
    static_assert(false, "Unrecognised OS");
#endif
