#pragma once

#include <memory>
#include <unordered_map>

#include "lexer/Token.hpp"
#include "parser/grammar/GrammarContext.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    struct Parser;
    struct ProgramParseResult;

    struct Grammar
    {
        std::unique_ptr<GrammarRule> fallback;
        std::unordered_map<Lexer::TokenType, std::unique_ptr<GrammarRule>>
            rules;

        Grammar();

        void add_rule(Lexer::TokenType type, std::unique_ptr<GrammarRule> rule);
        GrammarRule *get_rule(Lexer::TokenType type,
                              GrammarContext context) const;

        ParseResult partial_parse(Parser &parser, GrammarContext context);
        ProgramParseResult parse(Parser &parser);
    };

} // namespace Parser
