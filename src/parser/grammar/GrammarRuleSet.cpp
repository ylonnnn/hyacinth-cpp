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

        auto &lexer = parser.lexer();
        size_t prev_pos = lexer.position();

        for (auto &rule : rules_)
        {
            auto [node, errors] = rule->parse(parser);

            result.errors.insert(result.errors.end(),
                                 std::make_move_iterator(errors.begin()),
                                 std::make_move_iterator(errors.end()));

            if (errors.empty() && parser.state() != ParserState::Panic)
            {
                prev_pos = lexer.position();
                result.nodes.push_back(std::move(node));
            }
        }

        if (!result.errors.empty())
            lexer.rewind(prev_pos);

        return result;
    }

} // namespace Parser
