#include <unordered_map>
#include <variant>

#include "ast/NodeCollection.hpp"
#include "ast/Program.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/Grammar.hpp"
#include "parser/grammar/expr/Expr.hpp"

namespace Parser
{
    Grammar::Grammar() : fallback_(std::make_unique<ExprRule>()) {}

    GrammarRule *Grammar::fallback() const { return fallback_.get(); }

    void Grammar::add_rule(Lexer::TokenType type,
                           std::unique_ptr<GrammarRule> rule)
    {
        rules_.insert_or_assign(type, std::move(rule));
    }

    GrammarRule *Grammar::get_rule(Lexer::TokenType type) const
    {
        auto it = rules_.find(type);
        if (it == rules_.end())
            return nullptr;

        return it->second.get();
    }

    ParseResult Grammar::parse(Parser &parser)
    {
        Lexer::Lexer &lexer = parser.lexer();

        ParseResultStatus status = ParseResultStatus::Success;
        auto program_node = std::make_unique<AST::Program>(parser.program());

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

            // std::cout << "curr: " << *token << "\n";
            GrammarRule *rule = get_rule(token->type);
            if (rule == nullptr)
            {
                status = ParseResultStatus::Failed;
                parser.update_state(ParserState::Panic);

                diagnostics.push_back(Diagnostic::create_syntax_error(token));
                lexer.next();

                continue;
            }

            auto [p_status, p_node, p_diagnostics] = rule->parse(parser);

            collection_node->collection().push_back(std::move(p_node));

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
