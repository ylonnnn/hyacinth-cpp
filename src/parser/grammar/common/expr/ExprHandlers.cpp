#include <memory>

#include "parser/grammar/common/expr/Expr.hpp"
#include "parser/grammar/common/expr/ExprHandlers.hpp"

namespace Parser
{
    std::unique_ptr<AST::LiteralExpr>
    parse_literal(Parser &parser,
                  [[maybe_unused]] Diagnostic::DiagnosticList &diagnostics)
    {
        return std::make_unique<AST::LiteralExpr>(parser.lexer().current());
    }

    std::unique_ptr<AST::IdentifierExpr>
    parse_identifier(Parser &parser,
                     [[maybe_unused]] Diagnostic::DiagnosticList &diagnostics)
    {
        Lexer::Token &token = parser.lexer().current();

        return std::make_unique<AST::IdentifierExpr>(token);
    }

    std::unique_ptr<AST::BinaryExpr>
    parse_binary(Parser &parser, std::unique_ptr<AST::Expr> &left,
                 float right_bp,
                 [[maybe_unused]] Diagnostic::DiagnosticList &diagnostics)
    {
        Lexer::Token &operation = parser.lexer().current();

        Expr *expr_rule;
        if (auto ptr = dynamic_cast<Expr *>(parser.grammar().fallback()))
            expr_rule = ptr;

        return std::make_unique<AST::BinaryExpr>(
            std::move(left), operation,
            expr_rule->parse_expr(parser, right_bp).data);
    }

    std::unique_ptr<AST::UnaryExpr>
    parse_unary(Parser &parser,
                [[maybe_unused]] Diagnostic::DiagnosticList &diagnostics)
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
                float right_bp,
                [[maybe_unused]] Diagnostic::DiagnosticList &diagnostics)
    {
        (void)right_bp;

        return std::make_unique<AST::UnaryExpr>(
            AST::UnaryType::Post, parser.lexer().current(), std::move(left));
    }

} // namespace Parser
