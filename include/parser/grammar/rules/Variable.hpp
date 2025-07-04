#pragma once

#include "parser/grammar/GrammarRule.hpp"
#include "parser/Parser.hpp"

namespace Parser
{
    class VariableDefinition : public GrammarRule
    {
      public:
        VariableDefinition();

        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
