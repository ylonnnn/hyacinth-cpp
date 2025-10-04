#pragma once

#include "lexer/Token.hpp"
#include "parser/ParseResult.hpp"
#include "parser/grammar/GrammarContext.hpp"

namespace Parser
{
    struct Parser;

    struct GrammarRule
    {
        Lexer::TokenType token_type;
        GrammarContext context;

        GrammarRule(Lexer::TokenType token_type, GrammarContext context);
        virtual ~GrammarRule() = default;

        virtual ParseResult parse(Parser &parser) = 0;
        virtual void parse(Parser &parser, ParseResult &result) = 0;
    };

} // namespace Parser
