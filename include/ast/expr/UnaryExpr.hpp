#pragma once

#include "ast/expr/Expr.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    enum class UnaryType
    {
        Pre,
        Post
    };

    struct UnaryExpr : Expr
    {
        UnaryType unary_type;

        Lexer::Token &operation;
        std::unique_ptr<Expr> expr;

      public:
        UnaryExpr(UnaryType unary_type, Lexer::Token &operation,
                  std::unique_ptr<Expr> &&expr);
        ~UnaryExpr() override = default;

        void print(std::ostream &os, uint32_t tab) const override;
    };

} // namespace AST
