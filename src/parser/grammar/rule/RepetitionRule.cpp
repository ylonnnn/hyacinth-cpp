#include <iostream>

#include "ast/NodeCollection.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/rule/RepetitionRule.hpp"
#include "parser/typedef.hpp"

namespace Parser
{
    RepetitionGrammarRuleSet::RepetitionGrammarRuleSet()
        : GrammarRuleSet(Lexer::TokenTypes::Miscellaneous::EndOfFile)
    {
    }

    void RepetitionGrammarRuleSet::initialize_rules() {}

    ParseResult RepetitionGrammarRuleSet::parse(Parser &parser)
    {
        auto [status, nodes, diagnostics] = unbuilt_parse(parser);

        return {status,
                std::make_unique<AST::NodeCollection<AST::Node>>(
                    Program::Position(parser.lexer().peek()->position),
                    std::move(nodes)),
                std::move(diagnostics)};
    }

    RepetitionGrammarRule::RepetitionGrammarRule(
        std::unique_ptr<GrammarRuleSet> rule_set, size_t min, size_t max,
        std::optional<Lexer::TokenType> stop_token,
        std::optional<Lexer::TokenType> bound_token)
        : rule_set_(std::move(rule_set)), min_(min), max_(max),
          stop_token_(std::move(stop_token)),
          bound_token_(std::move(bound_token))
    {
    }

    RepetitionGrammarRule::RepetitionGrammarRule(
        std::vector<std::unique_ptr<GrammarRule>> &&rules, size_t min,
        size_t max, std::optional<Lexer::TokenType> stop_token,
        std::optional<Lexer::TokenType> bound_token)
        : rule_set_(std::make_unique<RepetitionGrammarRuleSet>()), min_(min),
          max_(max), stop_token_(std::move(stop_token)),
          bound_token_(std::move(bound_token))
    {
        for (auto &rule : rules)
            rule_set_->add_rule(std::move(rule));
    }

    ParseResult RepetitionGrammarRule::parse(Parser &parser)
    {
        size_t rule_count = rule_set_->rule_count();

        ParseResult result = {.status = ParseResultStatus::Success};
        result.diagnostics.reserve(rule_count);

        auto &lexer = parser.lexer();
        if (lexer.eof())
            return result;

        auto collection_node = std::make_unique<AST::NodeCollection<AST::Node>>(
            Program::Position(lexer.peek()->position),
            std::vector<std::unique_ptr<AST::Node>>());

        auto &collection = collection_node->collection();
        collection.reserve(rule_count);

        auto matches = 0;
        size_t initial_pos = lexer.position();

        while (matches < max_ && !lexer.eof())
        {
            Lexer::Token *next_token = lexer.peek();
            if (next_token == nullptr ||
                (stop_token_ && next_token->type == *stop_token_) ||
                (bound_token_ && next_token->type == *bound_token_))
                break;

            size_t prev_pos = lexer.position();
            auto [status, node, diagnostics] = rule_set_->parse(parser);

            if (initial_pos == lexer.position())
                break;

            if (status == ParseResultStatus::Success)
            {
                collection.push_back(std::move(node));
                matches++;

                continue;
            }

            result.diagnostics.insert(
                result.diagnostics.end(),
                std::make_move_iterator(diagnostics.begin()),
                std::make_move_iterator(diagnostics.end()));

            if (!stop_token_)
                continue;

            // Rewind to prevent the parser from going over the "Stop Token"
            lexer.rewind(initial_pos - 1);

            auto found = false;
            lexer.skip(
                [&](Lexer::Token &token)
                {
                    found = token.type == *stop_token_ ||
                            (bound_token_ && token.type == *bound_token_);

                    return found;
                });

            // std::cout << lexer.eof() << " | " << lexer.current() << "\n";

            // If the lexer reached did not reach EOF, the terminator token was
            // found, update the parser state to Synchronized
            if (found)
                parser.update_state(ParserState::Synchronized);

            // Otherwise, rewind to the initial position
            else
                lexer.rewind(prev_pos);

            result.status = ParseResultStatus::Failed;

            break;
        }

        if (matches < min_)
            lexer.rewind(initial_pos);

        result.node = std::move(collection_node);

        return result;
    }

} // namespace Parser
