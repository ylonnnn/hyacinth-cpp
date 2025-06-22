#pragma once

#include "parser/grammar/rule/RepetitionRule.hpp"

namespace Parser
{
    class OptionalGrammarRule : public RepetitionGrammarRule
    {
      public:
        OptionalGrammarRule(std::unique_ptr<GrammarRuleSet> rule_set);

        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
