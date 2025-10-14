#pragma once

#include "ast/expr/Expr.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    struct BinaryExpr : Expr
    {
        Lexer::Token &operation;
        std::unique_ptr<Expr> left;
        std::unique_ptr<Expr> right;

        BinaryExpr(Lexer::Token &operation, std::unique_ptr<Expr> &&left,
                   std::unique_ptr<Expr> &&right);
        ~BinaryExpr() override = default;

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
