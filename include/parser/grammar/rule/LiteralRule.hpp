#pragma once

#include "GrammarRule.hpp"
#include "lexer/Token.hpp"

namespace Parser
{
    class LiteralGrammarRule : public GrammarRule
    {
      private:
        Lexer::TokenType token_type_;

      public:
        LiteralGrammarRule(Lexer::TokenType token_type);

      public:
        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
