#pragma once

#include <memory>
#include <ostream>
#include <re2/re2.h>

#include "lexer/TokenType.hpp"
#include "program/Program.hpp"

namespace Lexer
{
    struct Token
    {
        std::string_view value;
        Program::Position position;
        TokenType type;

        friend std::ostream &operator<<(std::ostream &os, const Token &token);
    };

    struct TokenHandler
    {
        TokenType type;
        std::string pattern;
        std::unique_ptr<re2::RE2> regex = nullptr;

        TokenHandler(TokenType type, const char *pattern);

        const re2::RE2 &get_regex();
    };

    extern std::vector<TokenHandler> TOKEN_HANDLERS;

} // namespace Lexer
