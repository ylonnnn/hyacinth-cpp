#pragma once

#include <cstdint>
#include <string>

namespace Utils
{
    using TextStyle = const char *;

    namespace Colors
    {
        constexpr TextStyle Black = "\033[30m";
        constexpr TextStyle Red = "\033[31m";
        constexpr TextStyle Green = "\033[32m";
        constexpr TextStyle Yellow = "\033[33m";
        constexpr TextStyle Blue = "\033[34m";
        constexpr TextStyle Magenta = "\033[35m";
        constexpr TextStyle Cyan = "\033[36m";
        constexpr TextStyle White = "\033[37m";

        constexpr TextStyle BrightBlack = "\033[90m"; // Gray
        constexpr TextStyle BrightRed = "\033[91m";
        constexpr TextStyle BrightGreen = "\033[92m";
        constexpr TextStyle BrightYellow = "\033[93m";
        constexpr TextStyle BrightBlue = "\033[94m";
        constexpr TextStyle BrightMagenta = "\033[95m";
        constexpr TextStyle BrightCyan = "\033[96m";
        constexpr TextStyle BrightWhite = "\033[97m";

        constexpr TextStyle BackgroundRed = "\033[41m";
        constexpr TextStyle BackgroundGreen = "\033[42m";
        constexpr TextStyle BackgroundYellow = "\033[43m";
        constexpr TextStyle BackgroundBlue = "\033[44m";
        constexpr TextStyle BackgroundMagenta = "\033[45m";
        constexpr TextStyle BackgroundCyan = "\033[46m";
        constexpr TextStyle BackgroundWhite = "\033[47m";

        constexpr TextStyle BackgroundBrightBlack = "\033[100m";
        constexpr TextStyle BackgroundBrightRed = "\033[101m";
        constexpr TextStyle BackgroundBrightGreen = "\033[102m";
        constexpr TextStyle BackgroundBrightYellow = "\033[103m";
        constexpr TextStyle BackgroundBrightBlue = "\033[104m";
        constexpr TextStyle BackgroundBrightMagenta = "\033[105m";
        constexpr TextStyle BackgroundBrightCyan = "\033[106m";
        constexpr TextStyle BackgroundBrightWhite = "\033[107m";
    } // namespace Colors

    namespace Styles
    {
        constexpr TextStyle Reset = "\033[0m";
        constexpr TextStyle Dim = "\033[2m";
        constexpr TextStyle Underline = "\033[4m";
        constexpr TextStyle Blink = "\033[5m";
        constexpr TextStyle Reverse = "\033[7m";
        constexpr TextStyle Hidden = "\033[8m";
    } // namespace Styles

    std::string tab(uint8_t tab_count, uint8_t size = 2);

    size_t visible_size(const std::string &str);

} // namespace Utils
