#pragma once

#include "parser/typedef.hpp"

namespace Parser
{
    class Parser;

    class GrammarRule
    {
      public:
        virtual ~GrammarRule() = default;

      public:
        virtual ParseResult parse(Parser &parser) = 0;
    };

} // namespace Parser
