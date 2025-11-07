#include <codecvt>
#include <locale>

#include "utils/char.hpp"

namespace utils
{
    std::string utf32_to_utf8(char32_t code_point)
    {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        return converter.to_bytes(code_point);
    }

    char32_t utf8_to_utf32(const std::string &utf8_char)
    {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        std::u32string result = converter.from_bytes(utf8_char);
        if (result.size() != 1)
        {
            throw std::runtime_error(
                "UTF-8 sequence represents multiple characters");
        }
        return result[0];
    }

} // namespace utils
