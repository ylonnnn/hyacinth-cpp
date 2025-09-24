#pragma once

#include <iostream>

namespace utils
{
    inline void terminate(const std::string &message)
    {
        std::cerr << "[Hyacinth::Termination] " << message << std::endl;
        exit(EXIT_SUCCESS);
    }

} // namespace utils
