#pragma once

#include "parser/grammar/Grammar.hpp"

namespace Parser::Hyacinth
{
    constexpr auto IDENTIFIER = Lexer::TokenType::Identifier;

    // constexpr auto IMPORT = Lexer::TokenType::Import;
    // constexpr auto UNIT = Lexer::TokenType::Unit;

    // constexpr auto
    //     // pub
    //     PUBLIC = Reserved::Public,
    //     // priv
    //     PRIVATE = Reserved::Private,
    //     // prot
    //     PROTECTED = Reserved::Protected;

    // constexpr auto STRUCT = Reserved::Struct;

    // constexpr auto FUNCTION = Reserved::Function;
    constexpr auto VARIABLE = Lexer::TokenType::Var;

    constexpr auto TERMINATOR = Lexer::TokenType::Semicolon;
    constexpr auto PATH_SEP = Lexer::TokenType::DoubleColon;

    void initialize(Grammar &grammar);

} // namespace Parser::Hyacinth
