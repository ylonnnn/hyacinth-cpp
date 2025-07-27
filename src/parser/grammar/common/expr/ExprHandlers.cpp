#include <memory>

#include "diagnostic/ErrorDiagnostic.hpp"
#include "parser/grammar/common/expr/Expr.hpp"
#include "parser/grammar/common/expr/ExprHandlers.hpp"

namespace Parser
{
    std::unique_ptr<AST::LiteralExpr>
    parse_literal(Parser &parser, [[maybe_unused]] ExprParseResult &result)
    {
        return std::make_unique<AST::LiteralExpr>(parser.lexer().current());
    }

    std::unique_ptr<AST::IdentifierExpr>
    parse_identifier(Parser &parser, [[maybe_unused]] ExprParseResult &result)
    {
        Lexer::Token &token = parser.lexer().current();

        return std::make_unique<AST::IdentifierExpr>(token);
    }

    std::unique_ptr<AST::BinaryExpr>
    parse_binary(Parser &parser, std::unique_ptr<AST::Expr> &left,
                 float right_bp, [[maybe_unused]] ExprParseResult &result)
    {
        auto &lexer = parser.lexer();
        Lexer::Token &operation = lexer.current();

        Expr *expr_rule;
        if (auto ptr = dynamic_cast<Expr *>(parser.grammar().fallback()))
            expr_rule = ptr;

        ExprParseResult r_res = expr_rule->parse_expr(parser, right_bp);

        std::unique_ptr<AST::Expr> right = std::move(r_res.data);

        if (r_res.status == Core::ResultStatus::Fail)
            result.status = r_res.status;

        result.diagnostics.insert(
            result.diagnostics.end(),
            std::make_move_iterator(r_res.diagnostics.begin()),
            std::make_move_iterator(r_res.diagnostics.end()));

        if (right == nullptr)
        {
            auto node = AST::LiteralExpr(lexer.current());
            result.force_error(&node,
                               Diagnostic::ErrorTypes::Syntax::MissingValue,
                               std::string("Missing ") + Diagnostic::ERR_GEN +
                                   "VALUE" + Utils::Styles::Reset +
                                   " for the right-side of the expression",
                               "Missing here");
        }

        return std::make_unique<AST::BinaryExpr>(std::move(left), operation,
                                                 std::move(right));
    }

    std::unique_ptr<AST::UnaryExpr>
    parse_unary(Parser &parser, [[maybe_unused]] ExprParseResult &result)
    {
        Lexer::Token &operation = parser.lexer().current();

        Expr *expr_rule;
        if (auto ptr = dynamic_cast<Expr *>(parser.grammar().fallback()))
            expr_rule = ptr;

        return std::make_unique<AST::UnaryExpr>(
            AST::UnaryType::Pre, operation,
            expr_rule->parse_expr(parser, 0).data);
    }

    std::unique_ptr<AST::UnaryExpr>
    parse_unary(Parser &parser, std::unique_ptr<AST::Expr> &left,
                [[maybe_unused]] float right_bp,
                [[maybe_unused]] ExprParseResult &result)
    {
        return std::make_unique<AST::UnaryExpr>(
            AST::UnaryType::Post, parser.lexer().current(), std::move(left));
    }

    std::unique_ptr<AST::BinaryExpr>
    parse_memaccess(Parser &parser, std::unique_ptr<AST::Expr> &left,
                    float right_bp, ExprParseResult &result)
    {
        auto &lexer = parser.lexer();
        Lexer::Token &operation = lexer.current();

        Expr *expr_rule;
        if (auto ptr = dynamic_cast<Expr *>(parser.grammar().fallback()))
            expr_rule = ptr;

        ExprParseResult r_res = expr_rule->parse_expr(parser, right_bp);

        std::unique_ptr<AST::Expr> right = std::move(r_res.data);

        if (r_res.status == Core::ResultStatus::Fail)
            result.status = r_res.status;

        result.diagnostics.insert(
            result.diagnostics.end(),
            std::make_move_iterator(r_res.diagnostics.begin()),
            std::make_move_iterator(r_res.diagnostics.end()));

        if (right == nullptr)
        {
            auto node = AST::LiteralExpr(lexer.current());
            result.force_error(&node,
                               Diagnostic::ErrorTypes::Syntax::MissingValue,
                               std::string("Missing ") + Diagnostic::ERR_GEN +
                                   "ACCESSOR" + Utils::Styles::Reset +
                                   " for the the member access expression",
                               "Accessing nothing after the dot operator");
        }

        return std::make_unique<AST::BinaryExpr>(std::move(left), operation,
                                                 std::move(right));
    }

    std::unique_ptr<AST::FunctionCalLExpr>
    parse_fncall(Parser &parser, std::unique_ptr<AST::Expr> &left,
                 [[maybe_unused]] float right_bp, ExprParseResult &result)
    {
        auto &lexer = parser.lexer();

        Expr *expr_rule;
        if (auto ptr = dynamic_cast<Expr *>(parser.grammar().fallback()))
            expr_rule = ptr;

        std::vector<std::unique_ptr<AST::Expr>> arguments;
        arguments.reserve(8);

        auto expect_arg = true;
        Lexer::TokenType closing_token_type =
            Lexer::TokenTypes::Delimeter::ParenthesisClose;

        while (!parser.expect(closing_token_type, false))
        {
            if (expect_arg)
            {
                ExprParseResult expr_res = expr_rule->parse_expr(parser, 0);
                if (expr_res.data == nullptr)
                {
                    result.error(
                        Diagnostic::create_syntax_error(&lexer.current()));

                    return nullptr;
                }

                arguments.push_back(std::move(expr_res.data));
                expect_arg = false;
            }

            if (parser.expect(Lexer::TokenTypes::Delimeter::Comma, false))
            {
                lexer.next();
                expect_arg = true;

                continue;
            }

            break;
        }

        size_t fnc_end_pos;

        if (auto diagnostic = parser.expect_or_error(closing_token_type, false))
        {
            parser.panic();

            result.status = Core::ResultStatus::Fail;
            result.diagnostics.push_back(std::move(diagnostic));
        }

        else
        {
            Lexer::Token *close = lexer.next();
            fnc_end_pos = close->position.col + close->value.size();
        }

        // return std::make_unique<AST::BinaryExpr>(
        //     std::move(left), operation,
        //     expr_rule->parse_expr(parser, right_bp).data);

        auto node = std::make_unique<AST::FunctionCalLExpr>(
            std::move(left), std::move(arguments));

        node->set_end_pos(fnc_end_pos);

        return node;
    }

} // namespace Parser
