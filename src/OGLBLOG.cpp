#include "OGLBLOG.hpp"

#include <iostream>

Log::Log()
{
    debug = false;
}

void Log::setDebug(bool debug_on)
{
    debug = debug_on; 
}

void Log::e(const char* message)
{
    if ( DEFAULT_DEBUG_MODE )
        std::cerr << message << std::endl;
}

void Log::d(const char* message)
{
    if ( DEFAULT_DEBUG_MODE ) 
        std::cout << message << std::endl;
}

void Log::f(const char* message)
{
    if ( DEFAULT_DEBUG_MODE )
    {
        // TODO: Add file writing here
    }
}