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

        UnbuiltParseResult result = {.status = ParseResultStatus::Success};

        result.nodes.reserve(rule_count);
        result.diagnostics.reserve(rule_count);

        auto &lexer = parser.lexer();
        size_t prev_pos = lexer.position();

        for (auto &rule : rules_)
        {
            auto [status, node, diagnostics] = rule->parse(parser);

            result.diagnostics.insert(
                result.diagnostics.end(),
                std::make_move_iterator(diagnostics.begin()),
                std::make_move_iterator(diagnostics.end()));

            std::cout << "status: " << static_cast<int>(status)
                      << " | state: " << static_cast<int>(parser.state())
                      << "\n";
            if (status == ParseResultStatus::Success &&
                parser.state() != ParserState::Panic)
            {
                prev_pos = lexer.position();
                result.nodes.push_back(std::move(node));

                continue;
            }

            result.status = status;
        }

        if (result.status == ParseResultStatus::Failed &&
            parser.state() == ParserState::Panic)
            lexer.rewind(prev_pos);

        return result;
    }

} // namespace Parser
