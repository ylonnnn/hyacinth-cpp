
#include "ast/expr/BinaryExpr.hpp"
#include "ast/expr/FunctionCallExpr.hpp"
#include "ast/expr/IdentifierExpr.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "ast/expr/UnaryExpr.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/common/expr/Expr.hpp"

namespace Parser
{
    std::unique_ptr<AST::LiteralExpr> parse_literal(Parser &parser,
                                                    ExprParseResult &result);

    std::unique_ptr<AST::IdentifierExpr>
    parse_identifier(Parser &parser, ExprParseResult &result);

    std::unique_ptr<AST::BinaryExpr>
    parse_binary(Parser &parser, std::unique_ptr<AST::Expr> &left,
                 float right_bp, ExprParseResult &result);

    std::unique_ptr<AST::UnaryExpr> parse_unary(Parser &parser,
                                                ExprParseResult &result);
    std::unique_ptr<AST::UnaryExpr>
    parse_unary(Parser &parser, std::unique_ptr<AST::Expr> &left,
                float right_bp, ExprParseResult &result);

    std::unique_ptr<AST::FunctionCalLExpr>
    parse_fncall(Parser &parser, std::unique_ptr<AST::Expr> &left,
                 float right_bp, ExprParseResult &result);

} // namespace Parser
