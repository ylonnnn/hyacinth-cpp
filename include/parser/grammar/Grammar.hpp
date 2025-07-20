#pragma once

#include <memory>
#include <unordered_map>

#include "lexer/Token.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    class Parser;
    class ProgramParseResult;

    class Grammar
    {
      protected:
        std::unordered_map<Lexer::TokenType, GrammarRule *> global_rules_;
        std::unordered_map<Lexer::TokenType, std::unique_ptr<GrammarRule>>
            rules_;

        std::unique_ptr<GrammarRule> fallback_;

      public:
        Grammar();

      public:
        GrammarRule *fallback() const;

        void add_rule(Lexer::TokenType type, std::unique_ptr<GrammarRule> rule,
                      bool global = true);
        GrammarRule *get_rule(Lexer::TokenType type, bool global = true) const;

        ParseResult partial_parse(Parser &parser, bool global = true);
        ProgramParseResult parse(Parser &parser);
    };

} // namespace Parser
