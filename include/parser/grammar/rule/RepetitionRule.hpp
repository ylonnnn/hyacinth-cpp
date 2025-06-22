#pragma once

#include <cstdint>
#include <optional>

#include "GrammarRule.hpp"
#include "lexer/Token.hpp"
#include "parser/grammar/GrammarRuleSet.hpp"

namespace Parser
{
    class RepetitionGrammarRule : public GrammarRule
    {
      private:
        size_t min_, max_;
        std::optional<Lexer::TokenType> stop_token_;

      protected:
        std::unique_ptr<GrammarRuleSet> rule_set_;

      public:
        RepetitionGrammarRule(
            std::unique_ptr<GrammarRuleSet> rule_set, size_t min = 0,
            size_t max = SIZE_MAX,
            std::optional<Lexer::TokenType> stop_token = std::nullopt);

      public:
        virtual ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
