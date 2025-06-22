#pragma once

#include "parser/grammar/Grammar.hpp"

namespace Parser::Hyacinth
{
    constexpr auto FUNCTION_DEFINITION = Lexer::TokenTypes::Reserved::Function;

    void initialize(Grammar *grammar);

} // namespace Parser::Hyacinth
