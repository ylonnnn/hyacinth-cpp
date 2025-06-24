#include <unordered_map>
#include <variant>

#include "ast/NodeCollection.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/Grammar.hpp"
#include "parser/grammar/GrammarRuleSet.hpp"
#include "utils/control.hpp"

namespace Parser
{
    GrammarRuleSet *Grammar::get_rule_set(Lexer::TokenType token_type)
    {
        auto it = rule_sets_.find(token_type);

        if (it == rule_sets_.end())
            return nullptr;

        return it->second.get();
    }

    ParseResult Grammar::parse(Parser &parser)
    {
        Lexer::Lexer &lexer = parser.lexer();

        ParseResultStatus status = ParseResultStatus::Success;
        auto collection_node = std::make_unique<AST::NodeCollection<AST::Node>>(
            parser.program().position_at(1, 1),
            std::vector<std::unique_ptr<AST::Node>>());

        std::vector<std::unique_ptr<Diagnostic::Diagnostic>> diagnostics;
        diagnostics.reserve(lexer.size());

        while (!lexer.eof())
        {
            Lexer::Token *token = lexer.peek();
            bool is_eof =
                std::holds_alternative<Lexer::TokenTypes::Miscellaneous>(
                    token->type) &&
                std::get<Lexer::TokenTypes::Miscellaneous>(token->type) ==
                    Lexer::TokenTypes::Miscellaneous::EndOfFile;

            if (token == nullptr || is_eof)
                break;

            auto it = rule_sets_.find(token->type);

            if (it == rule_sets_.end())
            {
                if (parser.state() != ParserState::Panic)
                    diagnostics.push_back(
                        Diagnostic::create_syntax_error(token));

                break;
            }

            auto [p_status, node, p_diagnostics] = it->second->parse(parser);

            collection_node->collection().push_back(std::move(node));

            if (p_status == ParseResultStatus::Failed)
            {
                status = p_status;
                diagnostics.insert(
                    diagnostics.end(),
                    std::make_move_iterator(p_diagnostics.begin()),
                    std::make_move_iterator(p_diagnostics.end()));
            }
        }

        return {status, std::move(collection_node), std::move(diagnostics)};
    }

} // namespace Parser
