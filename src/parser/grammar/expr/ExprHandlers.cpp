#include <iostream>
#include <memory>

#include "parser/grammar/expr/Expr.hpp"
#include "parser/grammar/expr/ExprHandlers.hpp"

namespace Parser
{
    std::unique_ptr<AST::LiteralExpr> parse_literal(Parser &parser,
                                                    DiagnosticList &diagnostics)
    {
        return std::make_unique<AST::LiteralExpr>(parser.lexer().current());
    }

    std::unique_ptr<AST::IdentifierExpr>
    parse_identifier(Parser &parser, DiagnosticList &diagnostics)
    {
        Lexer::Token &token = parser.lexer().current();

        return std::make_unique<AST::IdentifierExpr>(token);
    }

    std::unique_ptr<AST::BinaryExpr>
    parse_binary(Parser &parser, std::unique_ptr<AST::Expr> &left,
                 float right_bp, DiagnosticList &diagnostics)
    {
        Lexer::Token &operation = parser.lexer().current();

        ExprRule *expr_rule;
        if (auto expr = dynamic_cast<ExprRule *>(parser.grammar().fallback()))
            expr_rule = expr;

        return std::make_unique<AST::BinaryExpr>(
            std::move(left), operation,
            expr_rule->parse_expr(parser, right_bp).node);
    }

    std::unique_ptr<AST::UnaryExpr> parse_unary(Parser &parser,
                                                DiagnosticList &diagnostics)
    {
        Lexer::Token &operation = parser.lexer().current();

        ExprRule *expr_rule;
        if (auto expr = dynamic_cast<ExprRule *>(parser.grammar().fallback()))
            expr_rule = expr;

        return std::make_unique<AST::UnaryExpr>(
            AST::UnaryType::Pre, operation,
            expr_rule->parse_expr(parser, 0).node);
    }

    std::unique_ptr<AST::UnaryExpr>
    parse_unary(Parser &parser, std::unique_ptr<AST::Expr> &left,
                float right_bp, DiagnosticList &diagnostics)
    {
        static_cast<void>(right_bp);

        return std::make_unique<AST::UnaryExpr>(
            AST::UnaryType::Post, parser.lexer().current(), std::move(left));
    }

} // namespace Parser
