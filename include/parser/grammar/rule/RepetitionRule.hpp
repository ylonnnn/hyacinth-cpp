#pragma once

#include <cstdint>
#include <optional>

#include "GrammarRule.hpp"
#include "lexer/Token.hpp"
#include "parser/grammar/GrammarRuleSet.hpp"

namespace Parser
{
    class RepetitionGrammarRuleSet : public GrammarRuleSet
    {
      public:
        RepetitionGrammarRuleSet();

        void initialize_rules() override;
        ParseResult parse(Parser &parser) override;
    };

    class RepetitionGrammarRule : public GrammarRule
    {
      private:
        size_t min_, max_;
        std::optional<Lexer::TokenType> stop_token_;
        std::optional<Lexer::TokenType> bound_token_;

      protected:
        std::unique_ptr<GrammarRuleSet> rule_set_;

      public:
        RepetitionGrammarRule(
            std::unique_ptr<GrammarRuleSet> rule_set, size_t min = 0,
            size_t max = SIZE_MAX,
            std::optional<Lexer::TokenType> stop_token = std::nullopt,
            std::optional<Lexer::TokenType> bound_token = std::nullopt);

        RepetitionGrammarRule(
            std::vector<std::unique_ptr<GrammarRule>> &&rules, size_t min = 0,
            size_t max = SIZE_MAX,
            std::optional<Lexer::TokenType> stop_token = std::nullopt,
            std::optional<Lexer::TokenType> bound_token = std::nullopt);

      public:
        virtual ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
