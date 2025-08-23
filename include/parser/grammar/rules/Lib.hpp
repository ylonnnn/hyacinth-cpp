#pragma once

#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    class LibBlock : public GrammarRule
    {
      public:
        LibBlock();

        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
