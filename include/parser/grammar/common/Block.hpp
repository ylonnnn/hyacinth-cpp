#pragma once

#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    struct Block : GrammarRule
    {
        Block();

        ParseResult parse(Parser &parser) override;

        void parse(Parser &parser, ParseResult &result) override;
        void parse(Parser &parser, GrammarContext context, ParseResult &result);

        void recover(Parser &parser) override;
    };

} // namespace Parser
