#pragma once

#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    class VariableDefinition : public GrammarRule
    {
      public:
        VariableDefinition();

        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
