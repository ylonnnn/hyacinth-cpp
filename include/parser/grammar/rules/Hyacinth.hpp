#pragma once

#include "parser/grammar/Grammar.hpp"

namespace Parser::Hyacinth
{
    using namespace Lexer::TokenTypes;

    constexpr auto IDENTIFIER = Primary::Identifier;
    constexpr auto TERMINATOR = Delimeter::Semicolon;

    constexpr auto FUNCTION = Reserved::Function;
    constexpr auto VARIABLE = Reserved::Declaration;

    void initialize(Grammar &grammar);

} // namespace Parser::Hyacinth
