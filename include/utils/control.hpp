#pragma once

#include <iostream>

namespace Utils
{
    inline void terminate(const char *message, int code)
    {
        std::cerr << message << std::endl;
        exit(code);
    }

} // namespace Utils
