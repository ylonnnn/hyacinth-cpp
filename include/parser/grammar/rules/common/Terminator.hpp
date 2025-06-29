#pragma once

#include "parser/grammar/GrammarRule.hpp"
#include "parser/Parser.hpp"

namespace Parser
{
    class Terminator : public GrammarRule
    {
      public:
        Terminator();

        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
