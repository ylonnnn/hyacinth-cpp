#include <iostream>
#include <unordered_map>
#include <variant>

#include "diagnostic/ErrorDiagnostic.hpp"
#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include "parser/ParseResult.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/Grammar.hpp"
#include "parser/grammar/common/expr/Expr.hpp"

namespace Parser
{
    Grammar::Grammar() : fallback_(std::make_unique<Expr>()) {}

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

    ProgramParseResult Grammar::parse(Parser &parser)
    {
        Lexer::Lexer &lexer = parser.lexer();
        ProgramParseResult result(
            parser, Core::ResultStatus::Success,
            std::make_unique<AST::Program>(parser.program()), {});

        result.diagnostics.reserve(lexer.size());

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

            GrammarRule *rule = get_rule(token->type);
            if (rule == nullptr)
            {
                result.error(Diagnostic::create_syntax_error(token));
                lexer.next();

                continue;
            }

            ParseResult p_res = rule->parse(parser);

            if (dynamic_cast<AST::DeclarationStmt *>(p_res.data.get()))
            {
                result.data->declarations().push_back(
                    std::unique_ptr<AST::DeclarationStmt>(
                        static_cast<AST::DeclarationStmt *>(
                            p_res.data.release())));
            }

            if (!p_res.diagnostics.empty())
                result.diagnostics.insert(
                    result.diagnostics.end(),
                    std::make_move_iterator(p_res.diagnostics.begin()),
                    std::make_move_iterator(p_res.diagnostics.end()));

            if (p_res.status == Core::ResultStatus::Fail)
                result.status = p_res.status;
        }

        return result;
    }

} // namespace Parser
