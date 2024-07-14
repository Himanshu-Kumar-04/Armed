#pragma once
#include <cstdio>

#ifdef ARM_DEBUG

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
    
    #define ARM_ASSERT(x, ...)  if(!(x)) {printf(__VA_ARGS__);__debugbreak();}
    #define ARM_ERROR(x, ...)   {printf(ANSI_COLOR_RED x ANSI_COLOR_RESET, __VA_ARGS__);}
    #define ARM_TRACE(x, ...)   {printf(ANSI_COLOR_GREEN x ANSI_COLOR_RESET, __VA_ARGS__);}
    #define ARM_WARNING(x, ...)   {printf(ANSI_COLOR_YELLOW x ANSI_COLOR_RESET, __VA_ARGS__);}
#else
    #define ARM_ASSERT(x, ...) 
    #define ARM_ERROR(x, ...)  
    #define ARM_TRACE(x, ...)  
    #define ARM_WARNING(x, ...)
#endif // ARM_DEBUG