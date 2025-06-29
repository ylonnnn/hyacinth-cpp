#include <cstdint>
#include <re2/re2.h>
#include <string>

#include "utils/style.hpp"

namespace Utils
{
    std::string tab(uint8_t tab_count, uint8_t size)
    {
        return std::string(tab_count * size, ' ');
    }

    static re2::RE2 style_regex("\033\\[[0-9;]*m");

    size_t visible_size(const std::string &str)
    {
        std::string stripped = str;
        re2::RE2::GlobalReplace(&stripped, style_regex, "");

        return stripped.size();
    }

} // namespace Utils
