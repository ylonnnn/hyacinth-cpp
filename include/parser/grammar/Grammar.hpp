#pragma once

#include <memory>
#include <type_traits>
#include <unordered_map>

#include "lexer/Token.hpp"
#include "parser/grammar/GrammarRuleSet.hpp"

namespace Parser
{
    class Parser;

    class Grammar
    {
      protected:
        std::unordered_map<Lexer::TokenType, std::unique_ptr<GrammarRuleSet>>
            rule_sets_;

      public:
        Grammar() = default;

      public:
        template <typename T,
                  typename = std::enable_if<
                      std::is_base_of<GrammarRuleSet, T>::value, T *>>
        T *make_rule_set(Lexer::TokenType token_type)
        {
            auto [it, _] = rule_sets_.insert_or_assign(
                token_type, std::move(std::make_unique<T>(token_type)));
            return static_cast<T *>(it->second.get());
        }

        GrammarRuleSet *get_rule_set(Lexer::TokenType token_type);

        ParseResult parse(Parser &parser);
    };

} // namespace Parser
