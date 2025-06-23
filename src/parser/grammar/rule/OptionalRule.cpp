#include "parser/grammar/rule/OptionalRule.hpp"
#include "ast/NodeCollection.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRuleSet.hpp"
#include <iostream>

namespace Parser
{
    OptionalGrammarRuleSet::OptionalGrammarRuleSet()
        : GrammarRuleSet(Lexer::TokenTypes::Miscellaneous::EndOfFile)
    {
    }

    void OptionalGrammarRuleSet::initialize_rules() {}

    ParseResult OptionalGrammarRuleSet::parse(Parser &parser)
    {
        auto [nodes, errors] = unbuilt_parse(parser);

        return {std::make_unique<AST::NodeCollection<AST::Node>>(
                    Program::Position(parser.lexer().peek()->position),
                    std::move(nodes)),
                std::move(errors)};
    }

    OptionalGrammarRule::OptionalGrammarRule(
        std::unique_ptr<GrammarRuleSet> rule_set)
        : rule_set_(std::move(rule_set))
    {
    }

    OptionalGrammarRule::OptionalGrammarRule(
        std::vector<std::unique_ptr<GrammarRule>> &&rules)
        : rule_set_(std::make_unique<OptionalGrammarRuleSet>())
    {
        for (auto &rule : rules)
            rule_set_->add_rule(std::move(rule));
    }

    ParseResult OptionalGrammarRule::parse(Parser &parser)
    {
        auto &lexer = parser.lexer();

        size_t initial_pos = lexer.position();
        ParseResult result = rule_set_->parse(parser);

        if (!result.errors.empty())
        {
            result.node = nullptr;

            // If the current position and the initial position are the same,
            // the parser did not meaningfully moved forward. Potentially due to
            // absence of the provided rule set.
            if (lexer.position() == initial_pos)
            {
                result.errors.clear();
                parser.update_state(ParserState::Normal);
            }
        }

        return result;
    }

}; // namespace Parser
