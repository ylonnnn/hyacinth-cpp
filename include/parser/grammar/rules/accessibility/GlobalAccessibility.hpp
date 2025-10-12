#pragma once

#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    struct GlobalAccessibility : GrammarRule
    {
        GlobalAccessibility(Lexer::TokenType token_type);

        ParseResult parse(Parser &parser) override;
        void parse(Parser &parser, ParseResult &result) override;
    };

} // namespace Parser
