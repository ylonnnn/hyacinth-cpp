#pragma once

#include "core/value/Value.hpp"
#include "lexer/Token.hpp"

namespace utils
{
    Core::Value parse_val(Lexer::Token &raw_token);

    template <typename T> constexpr inline T *as(Core::Value &value)
    {
        return std::get_if<T>(&value);
    }

    template <typename T> constexpr inline const T *as(const Core::Value &value)
    {
        return std::get_if<T>(&value);
    }

} // namespace utils
