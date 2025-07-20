#pragma once

#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    class Block : public GrammarRule
    {
      private:
        std::pair<Lexer::TokenType, Lexer::TokenType> pair_;

      public:
        Block(std::pair<Lexer::TokenType, Lexer::TokenType> pair);

        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
