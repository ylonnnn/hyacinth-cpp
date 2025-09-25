#pragma once

#include <cctype>
#include <cstdint>

namespace utils
{
    static inline bool is_digit_of(uint32_t base, char c)
    {
        switch (base)
        {
            case 2:
                return c == '0' || c == '1';
            case 8:
                return c >= '0' && c <= '7';
            case 10:
                return std::isdigit(c);
            case 16:
                return std::isdigit(c) || (c >= 'a' && c <= 'f') ||
                       (c >= 'A' && c <= 'F');

            default:
                return false;
        }
    }

} // namespace utils
