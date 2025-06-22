#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRuleSet.hpp"
#include "parser/typedef.hpp"

namespace Parser
{
    GrammarRuleSet::GrammarRuleSet(Lexer::TokenType token_type)
        : token_type_(token_type)
    {
        rules_.reserve(32);
    }

    Lexer::TokenType GrammarRuleSet::token_type() const { return token_type_; }

    size_t GrammarRuleSet::rule_count() const { return rules_.size(); }

    UnbuiltParseResult GrammarRuleSet::unbuilt_parse(Parser &parser)
    {
        size_t rule_count = rules_.size();

        UnbuiltParseResult result;

        result.nodes.reserve(rule_count);
        result.errors.reserve(rule_count);

        // std::cout << "[[[START]]] set {\n";
        // std::cout << "rules: " << rule_count << "\n";

        for (auto &rule : rules_)
        {
            auto [node, errors] = rule->parse(parser);
            if (errors.empty())
            {
                result.nodes.push_back(std::move(node));
                continue;
            }

            result.errors.insert(result.errors.end(),
                                 std::make_move_iterator(errors.begin()),
                                 std::make_move_iterator(errors.end()));
        }

        // std::cout << "[[[END]]] set }\n";

        return result;
    }

} // namespace Parser
