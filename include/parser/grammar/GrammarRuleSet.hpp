#pragma once

#include "lexer/Token.hpp"
#include "parser/grammar/rule/GrammarRule.hpp"
#include "parser/typedef.hpp"

namespace Parser
{
    class Grammar;

    class GrammarRuleSet
    {
      protected:
        Lexer::TokenType token_type_;
        std::vector<std::unique_ptr<GrammarRule>> rules_;

      protected:
        // Grammar *grammar_;

      public:
        GrammarRuleSet(Lexer::TokenType token_type);
        virtual ~GrammarRuleSet() = default;

        GrammarRuleSet(const GrammarRuleSet &) = delete;
        GrammarRuleSet &operator=(const GrammarRuleSet &) = delete;

        GrammarRuleSet(GrammarRuleSet &&) noexcept = default;
        GrammarRuleSet &operator=(GrammarRuleSet &&) noexcept = default;

      public:
        Lexer::TokenType token_type() const;
        size_t rule_count() const;

        template <typename T, typename... Args>
        GrammarRuleSet &make_rule(Args &&...args)
        {
            rules_.push_back(std::make_unique<T>(std::forward<Args>(args)...));
            return *this;
        }

        UnbuiltParseResult unbuilt_parse(Parser &parser);

        virtual void initialize_rules() = 0;
        virtual ParseResult parse(Parser &parser) = 0;
    };

} // namespace Parser
