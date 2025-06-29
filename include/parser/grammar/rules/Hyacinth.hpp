#pragma once

#include "parser/grammar/Grammar.hpp"

namespace Parser::Hyacinth
{
    constexpr auto IDENTIFIER = Lexer::TokenTypes::Primary::Identifier;
    constexpr auto TERMINATOR = Lexer::TokenTypes::Delimeter::Semicolon;

    constexpr auto FUNCTION = Lexer::TokenTypes::Reserved::Function;
    constexpr auto VARIABLE = IDENTIFIER;

    void initialize(Grammar &grammar);

} // namespace Parser::Hyacinth
