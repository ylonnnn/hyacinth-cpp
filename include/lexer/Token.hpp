#pragma once

#include <ostream>

#include "core/position/Position.hpp"
#include "lexer/TokenType.hpp"

namespace Lexer
{
    struct Token
    {
        std::string_view value;
        Core::PositionRange range;
        TokenType type;

        Token(std::string_view value, Core::PositionRange &&range,
              TokenType type);

        friend std::ostream &operator<<(std::ostream &os, const Token &token);
    };

} // namespace Lexer
