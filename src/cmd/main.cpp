#include <iostream>

#include "../helper/Log.hpp"

int main()
{
    
    Log::setLevel(Log::ALL);

    Log::debug("A debug message");
    Log::info("An information message");
    Log::warning("An warning message");
    Log::error("An error message");

    return 0;
}