#pragma once

#include <memory>
#include <unordered_map>

#include "lexer/Token.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    class Parser;

    class Grammar
    {
      protected:
        std::unordered_map<Lexer::TokenType, std::unique_ptr<GrammarRule>>
            rules_;
        std::unique_ptr<GrammarRule> fallback_;

      public:
        Grammar();

      public:
        GrammarRule *fallback() const;

        void add_rule(Lexer::TokenType type, std::unique_ptr<GrammarRule> rule);
        GrammarRule *get_rule(Lexer::TokenType type) const;

        ParseResult parse(Parser &parser);
    };

} // namespace Parser
