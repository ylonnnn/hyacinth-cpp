#pragma once

#include "parser/grammar/rule/ChoiceRule.hpp"

namespace Parser::Hyacinth
{
    class MutabilityChoice : public ChoiceGrammarRule
    {
      public:
        MutabilityChoice();
        
        void initialize();

        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser::Hyacinth
