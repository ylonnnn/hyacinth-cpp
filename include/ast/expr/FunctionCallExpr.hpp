#pragma once

#include <vector>

#include "ast/expr/Expr.hpp"
// #include "core/symbol/FunctionSymbol.hpp"

namespace AST
{
    struct FunctionCallExpr : Expr
    {
        std::unique_ptr<Expr> callee;
        std::vector<std::unique_ptr<Expr>> arguments;

        // Core::FunctionSymbol *fn_symbol_ = nullptr;

        FunctionCallExpr(std::unique_ptr<Expr> callee,
                         std::vector<std::unique_ptr<Expr>> arguments);

        // void set_fn_symbol(Core::FunctionSymbol *fn_symbol);

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
