#pragma once

#include "parser/grammar/GrammarRuleSet.hpp"

namespace Parser
{
    class OptionalGrammarRuleSet : public GrammarRuleSet
    {
      public:
        OptionalGrammarRuleSet();

        void initialize_rules() override;
        ParseResult parse(Parser &parser) override;
    };

    class OptionalGrammarRule : public GrammarRule
    {
      private:
        std::unique_ptr<GrammarRuleSet> rule_set_;

      public:
        OptionalGrammarRule(std::unique_ptr<GrammarRuleSet> rule_set);
        OptionalGrammarRule(std::vector<std::unique_ptr<GrammarRule>> &&rule);

      public:
        OptionalGrammarRuleSet &rule_set();

        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
