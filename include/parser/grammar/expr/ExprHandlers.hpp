#pragma once

#include "ast/expr/BinaryExpr.hpp"
#include "ast/expr/IdentifierExpr.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "ast/expr/UnaryExpr.hpp"
#include "parser/Parser.hpp"

namespace Parser
{
    std::unique_ptr<AST::LiteralExpr>
    parse_literal(Parser &parser, DiagnosticList &diagnostics);

    std::unique_ptr<AST::IdentifierExpr>
    parse_identifier(Parser &parser, DiagnosticList &diagnostics);

    std::unique_ptr<AST::BinaryExpr>
    parse_binary(Parser &parser, std::unique_ptr<AST::Expr> &left,
                 float right_bp, DiagnosticList &diagnostics);

    std::unique_ptr<AST::UnaryExpr> parse_unary(Parser &parser,
                                                DiagnosticList &diagnostics);
    std::unique_ptr<AST::UnaryExpr>
    parse_unary(Parser &parser, std::unique_ptr<AST::Expr> &left,
                float right_bp, DiagnosticList &diagnostics);

} // namespace Parser
