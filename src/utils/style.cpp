#include <cstdint>
#include <string>

#include "utils/style.hpp"

namespace Utils
{
    std::string tab(uint8_t tab_count, uint8_t size)
    {
        return std::string(tab_count * size, ' ');
    }

    size_t visible_size(const std::string &str)
    {
        std::string stripped;
        stripped.reserve(str.size());

        for (size_t i = 0; i < str.size(); ++i)
        {
            if (str[i] == '\033' && i + 1 < str.size() && str[i + 1] == '[')
            {
                i += 2;
                while (i < str.size() &&
                       (std::isdigit(str[i]) || str[i] == ';'))
                    ++i;
                if (i < str.size() && str[i] == 'm')
                    continue;
            }

            stripped += str[i];
        }

        return stripped.size();
    }

} // namespace Utils
