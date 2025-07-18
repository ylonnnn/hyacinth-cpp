#pragma once

#include "lexer/Token.hpp"
#include "parser/ParseResult.hpp"

namespace Parser
{
    class Parser;

    class GrammarRule
    {
      protected:
        Lexer::TokenType token_type_;

      public:
        GrammarRule(Lexer::TokenType token_type);
        virtual ~GrammarRule() = default;

        Lexer::TokenType token_type() const;

        virtual ParseResult parse(Parser &parser) = 0;
    };

} // namespace Parser
