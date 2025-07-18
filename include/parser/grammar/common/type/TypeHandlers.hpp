#pragma once

#include "ast/type/ConstantType.hpp"
#include "ast/type/GenericType.hpp"
#include "ast/type/SimpleType.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/common/type/Type.hpp"

namespace Parser
{
    std::unique_ptr<AST::SimpleType> parse_simple(Parser &parser,
                                                  TypeParseResult &result);

    std::unique_ptr<AST::ConstantType> parse_constant(Parser &parser,
                                                      TypeParseResult &result);

    std::unique_ptr<AST::GenericType>
    parse_generic(Parser &parser, std::unique_ptr<AST::Type> &constructor,
                  float right_bp, TypeParseResult &result);

    // std::unique_ptr<AST::IdentifierExpr>
    // parse_identifier(Parser &parser, Diagnostic::DiagnosticList &diagnostics);

    // std::unique_ptr<AST::BinaryExpr>
    // parse_binary(Parser &parser, std::unique_ptr<AST::Expr> &left,
    //              float right_bp, Diagnostic::DiagnosticList &diagnostics);

    // std::unique_ptr<AST::UnaryExpr> parse_unary(Parser &parser,
    //                                             Diagnostic::DiagnosticList &diagnostics);
    // std::unique_ptr<AST::UnaryExpr>
    // parse_unary(Parser &parser, std::unique_ptr<AST::Expr> &left,
    //             float right_bp, Diagnostic::DiagnosticList &diagnostics);
} // namespace Parser
