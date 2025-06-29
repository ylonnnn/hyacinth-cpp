#pragma once

#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/rules/common/Mutability.hpp"
#include "parser/grammar/rules/common/Terminator.hpp"

namespace Parser
{
    class IdentifierInitialization : public GrammarRule
    {
      protected:
        Mutability mutability_;
        Terminator terminator_;

      public:
        IdentifierInitialization();

        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
