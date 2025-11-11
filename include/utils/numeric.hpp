#pragma once

#include <cctype>
#include <cstdint>
#include <string>

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

    static inline uint32_t as_digit_of(char c)
    {
        if (c >= '0' && c <= '9')
            return (c - '0');

        char ch = std::tolower(c);
        return ch >= 'a' && ch <= 'f' ? (ch - 'a') : 0;
    }

    static inline double to_numeric(const std::string &val, uint32_t base = 10)
    {
        auto n = 0.0;

        auto at_frac = false;
        auto frac_base = 1;

        for (char c : val)
        {
            // Separator
            if (c == '.')
            {
                at_frac = true;
                continue;
            }

            uint32_t cv = as_digit_of(c);

            if (at_frac)
            {
                frac_base *= base;
                n += (static_cast<double>(cv) / frac_base);
            }
            else
            {
                n *= base;
                n += cv;
            }
        }

        return n;
    }

} // namespace utils
