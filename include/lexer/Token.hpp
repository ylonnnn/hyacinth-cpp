#pragma once

#include <ostream>

#include "core/program/Program.hpp"
#include "lexer/TokenType.hpp"

namespace Lexer
{
    struct Token
    {
        std::string_view value;
        Core::Position position, end_position;
        TokenType type;

        friend std::ostream &operator<<(std::ostream &os, const Token &token);
    };

} // namespace Lexer
