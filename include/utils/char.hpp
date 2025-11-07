#pragma once

#include <string>

namespace utils
{
    std::string utf32_to_utf8(char32_t code_point);

    char32_t utf8_to_utf32(const std::string &utf8_char);

} // namespace utils
