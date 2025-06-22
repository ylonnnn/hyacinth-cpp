#include "parser/grammar/rule/OptionalRule.hpp"
#include "ast/NodeCollection.hpp"
#include "parser/Parser.hpp"
#include <iostream>

namespace Parser
{
    OptionalGrammarRule::OptionalGrammarRule(
        std::unique_ptr<GrammarRuleSet> rule_set)
        : RepetitionGrammarRule(std::move(rule_set), 0, 1)
    {
    }

    ParseResult OptionalGrammarRule::parse(Parser &parser)
    {
        ParseResult result;
        auto [node, errors] = RepetitionGrammarRule::parse(parser);

        if (!errors.empty())
            result.errors.insert(result.errors.end(),
                                 std::make_move_iterator(errors.begin()),
                                 std::make_move_iterator(errors.end()));

        if (auto *node_ptr =
                dynamic_cast<AST::NodeCollection<AST::Node> *>(node.get()))
        {
            std::vector<std::unique_ptr<AST::Node>> &collection =
                node_ptr->collection();

            std::cout << "collection.size(): " << collection.size() << "\n";

            if (collection.empty())
            {
                result.errors.clear();
                parser.update_state(ParserState::Normal);

                return result;
            }

            std::cout << "(optional) has value\n";
            result.node = std::move(collection.front());

            return result;
        }

        return result;
    }

}; // namespace Parser
