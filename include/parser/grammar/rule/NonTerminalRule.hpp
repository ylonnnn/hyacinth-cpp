#pragma once

#include "parser/grammar/GrammarRuleSet.hpp"
#include "parser/grammar/rule/GrammarRule.hpp"

namespace Parser
{
    class NonTerminalGrammarRule : public GrammarRule
    {
      protected:
        std::unique_ptr<GrammarRuleSet> rule_set_;

      public:
        NonTerminalGrammarRule(std::unique_ptr<GrammarRuleSet> rule_set);

        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
