#include <iostream>

#include "ast/NodeCollection.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/rule/RepetitionRule.hpp"
#include "parser/typedef.hpp"

namespace Parser
{
    RepetitionGrammarRule::RepetitionGrammarRule(
        std::unique_ptr<GrammarRuleSet> rule_set, size_t min, size_t max,
        std::optional<Lexer::TokenType> stop_token)
        : rule_set_(std::move(rule_set)), min_(min), max_(max),
          stop_token_(std::move(stop_token))
    {
    }

    ParseResult RepetitionGrammarRule::parse(Parser &parser)
    {
        size_t rule_count = rule_set_->rule_count();

        ParseResult result;
        result.errors.reserve(rule_count);

        auto &lexer = parser.lexer();
        if (lexer.eof())
            return result;

        auto collection_node = std::make_unique<AST::NodeCollection<AST::Node>>(
            Program::Position(lexer.peek()->position),
            std::vector<std::unique_ptr<AST::Node>>());

        auto &collection = collection_node->collection();
        collection.reserve(rule_count);

        auto matches = 0;

        while (matches < max_ && !lexer.eof())
        {
            Lexer::Token *next_token = lexer.peek();
            if (next_token == nullptr ||
                (stop_token_ && next_token->type == *stop_token_))
                break;

            size_t initial_pos = lexer.position();
            auto [node, errors] = rule_set_->parse(parser);

            if (lexer.position() == initial_pos)
                break;

            // if (errors.empty() && parser.state() != ParserState::Panic)
            if (errors.empty())
            {
                collection.push_back(std::move(node));
                matches++;

                continue;
            }

            result.errors.insert(result.errors.end(),
                                 std::make_move_iterator(errors.begin()),
                                 std::make_move_iterator(errors.end()));

            std::cout << !!stop_token_ << "\n";
            if (!stop_token_)
                continue;

            // Rewind to prevent the parser from going over the "Stop Token"
            lexer.rewind(initial_pos);

            lexer.skip([&](Lexer::Token &token)
                       { return stop_token_ && token.type == *stop_token_; });

            // If the lexer reached did not reach EOF, the terminator token was
            // found, update the parser state to Synchronized
            if (!lexer.eof())
                parser.update_state(ParserState::Synchronized);

            // Otherwise, rewind to the initial position
            else
                lexer.rewind(initial_pos);

            break;
        }

        result.node = std::move(collection_node);

        return result;
    }

} // namespace Parser
