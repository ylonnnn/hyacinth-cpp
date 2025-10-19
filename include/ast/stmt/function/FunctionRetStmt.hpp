#pragma once

#include "ast/expr/Expr.hpp"
#include "ast/stmt/Stmt.hpp"

namespace AST
{
    struct FunctionReturnStmt : Stmt
    {
        std::unique_ptr<Expr> return_value;

        FunctionReturnStmt(Core::Position &position,
                           std::unique_ptr<Expr> &&value = nullptr);

        void print(std::ostream &os, uint32_t tab) const override;
    };

} // namespace AST
