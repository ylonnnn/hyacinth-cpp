#pragma once

#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    struct Terminator : public GrammarRule
    {
      public:
        Terminator();

        ParseResult parse(Parser &parser) override;
        void parse(Parser &parser, ParseResult &result) override;
    };

} // namespace Parser
