#include <algorithm>
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
    Grammar::Grammar() : fallback_(std::make_unique<Expr>())
    {
        global_rules_.reserve(8);
        rules_.reserve(16);
    }

    GrammarRule *Grammar::fallback() const { return fallback_.get(); }

    void Grammar::add_rule(Lexer::TokenType type,
                           std::unique_ptr<GrammarRule> rule, bool global)
    {
        if (global)
            global_rules_.insert_or_assign(type, rule.get());

        rules_.insert_or_assign(type, std::move(rule));
    }

    GrammarRule *Grammar::get_rule(Lexer::TokenType type, bool global) const
    {
        GrammarRule *rule;

        if (global)
        {
            auto it = global_rules_.find(type);
            rule = it == global_rules_.end() ? nullptr : it->second;
        }

        else
        {
            auto it = rules_.find(type);
            rule = it == rules_.end() ? nullptr : it->second.get();
        }

        return rule;
    }

    ParseResult Grammar::partial_parse(Parser &parser, bool global)
    {
        Lexer::Lexer &lexer = parser.lexer();
        ParseResult result(parser, Core::ResultStatus::Success, nullptr, {});

        result.diagnostics.reserve(
            std::max(static_cast<size_t>(32), lexer.size() / 8));

        if (lexer.eof(false))
            return result;

        Lexer::Token *token = lexer.peek();
        if (token == nullptr)
            return result;

        GrammarRule *rule = get_rule(token->type, global);
        if (rule == nullptr)
        {
            if (!global)
                rule = fallback_.get();

            else
            {
                result.error(Diagnostic::create_syntax_error(token));
                lexer.next();

                return result;
            }
        }

        ParseResult p_res = rule->parse(parser);

        result.data = std::move(p_res.data);

        result.diagnostics.insert(
            result.diagnostics.end(),
            std::make_move_iterator(p_res.diagnostics.begin()),
            std::make_move_iterator(p_res.diagnostics.end()));

        if (p_res.status == Core::ResultStatus::Fail)
            result.status = p_res.status;

        return result;
    }

    ProgramParseResult Grammar::parse(Parser &parser)
    {
        Lexer::Lexer &lexer = parser.lexer();
        ProgramParseResult result(
            parser, Core::ResultStatus::Success,
            std::make_unique<AST::Program>(parser.program()), {});

        result.diagnostics.reserve(lexer.size());

        while (!lexer.eof(false))
        {
            ParseResult p_res = partial_parse(parser);

            if (p_res.data)
                if (auto ptr = dynamic_cast<AST::DeclarationStmt *>(
                        p_res.data.release()))
                    result.data->declarations().push_back(
                        std::unique_ptr<AST::DeclarationStmt>(ptr));

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
