#pragma once

#include "parser/grammar/GrammarRuleSet.hpp"
#include "parser/grammar/rule/GrammarRule.hpp"

namespace Parser
{
    class ChoiceGrammarRuleSet : public GrammarRuleSet
    {
      public:
        ChoiceGrammarRuleSet();

        void initialize_rules() override;
        ParseResult parse(Parser &parser) override;
    };

    class ChoiceGrammarRule : public GrammarRule
    {
      protected:
        std::vector<std::unique_ptr<GrammarRuleSet>> rule_sets_;

      public:
        ChoiceGrammarRule(
            std::vector<std::unique_ptr<GrammarRuleSet>> &&rule_sets);

        ChoiceGrammarRule(
            std::vector<std::vector<std::unique_ptr<GrammarRule>>> &&rules);

        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
