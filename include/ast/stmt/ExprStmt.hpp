#pragma once

#include "ast/expr/Expr.hpp"
#include "ast/stmt/Stmt.hpp"

namespace AST
{
    struct ExprStmt : Stmt
    {
        std::unique_ptr<AST::Expr> expr;

        ExprStmt(std::unique_ptr<Expr> expr);
        ~ExprStmt() = default;

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
