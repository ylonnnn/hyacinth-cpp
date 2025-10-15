#pragma once

#include "parser/grammar/Grammar.hpp"

namespace Parser::Hyacinth
{
    constexpr auto IMPORT = Lexer::TokenType::Import;
    constexpr auto PETAL = Lexer::TokenType::Petal;

    constexpr auto
        // pub
        PUBLIC = Lexer::TokenType::Pub,
        // prot
        PROTECTED = Lexer::TokenType::Prot;

    constexpr auto STRUCT = Lexer::TokenType::Struct;

    constexpr auto FUNCTION = Lexer::TokenType::Fn;
    constexpr auto FN_RETURN = Lexer::TokenType::Return;

    constexpr auto VARIABLE = Lexer::TokenType::Var;

    constexpr auto BLOCK = Lexer::TokenType::LeftBrace;

    constexpr auto TERMINATOR = Lexer::TokenType::Semicolon;
    constexpr auto PATH_SEP = Lexer::TokenType::DoubleColon;

    void initialize(Grammar &grammar);

} // namespace Parser::Hyacinth
