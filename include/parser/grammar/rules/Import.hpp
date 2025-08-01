#pragma once

#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    class ImportStatement : public GrammarRule
    {
      public:
        ImportStatement();

        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
