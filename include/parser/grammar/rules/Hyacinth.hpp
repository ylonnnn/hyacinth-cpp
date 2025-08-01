#pragma once

#include "parser/grammar/Grammar.hpp"

namespace Parser::Hyacinth
{
    using namespace Lexer::TokenTypes;

    constexpr auto IDENTIFIER = Primary::Identifier;
    constexpr auto TERMINATOR = Delimeter::Semicolon;

    constexpr auto IMPORT = Reserved::Import;

    constexpr auto
        // pub
        PUBLIC = Reserved::Public,
        // priv
        PRIVATE = Reserved::Private,
        // prot
        PROTECTED = Reserved::Protected;

    constexpr auto STRUCT = Reserved::Struct;

    constexpr auto FUNCTION = Reserved::Function;
    constexpr auto VARIABLE = Reserved::Declaration;

    void initialize(Grammar &grammar);

} // namespace Parser::Hyacinth
