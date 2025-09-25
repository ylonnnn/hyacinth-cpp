#pragma once

#include <iostream>
#include <string>

namespace utils
{
    static inline void todo(const std::string &message)
    {
        std::cout << "[TODO] " << message << "\n";
    }

} // namespace utils
