#pragma once

#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    class GlobalAccessibility : public GrammarRule
    {
      public:
        GlobalAccessibility(Lexer::TokenType token_type);

        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
