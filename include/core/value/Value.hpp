#pragma once

#include <cstdint>
#include <string>
#include <variant>

namespace Core
{
    struct null
    {
    };

    using Value =
        std::variant<null, int64_t, uint64_t, double, char, bool, std::string>;

    template <typename T> Value make_value(T raw)
    {
        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, int64_t>)
            return int64_t(raw);

        else if constexpr (std::is_same_v<T, unsigned int> ||
                           std::is_same_v<T, uint64_t>)
            return uint64_t(raw);

        return raw;
    }

} // namespace Core
