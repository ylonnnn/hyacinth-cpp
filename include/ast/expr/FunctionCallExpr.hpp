#pragma once

#include <vector>

#include "ast/expr/Expr.hpp"
#include "core/symbol/FunctionSymbol.hpp"

namespace AST
{
    class FunctionCallExpr : public Expr
    {
      private:
        std::unique_ptr<Expr> callee_;
        std::vector<std::unique_ptr<Expr>> arguments_;

        Core::FunctionSymbol *fn_symbol_ = nullptr;

      public:
        FunctionCallExpr(std::unique_ptr<Expr> callee,
                         std::vector<std::unique_ptr<Expr>> arguments);

        Expr &callee();
        std::vector<std::unique_ptr<Expr>> &arguments();

        Core::FunctionSymbol *fn_symbol();
        const Core::FunctionSymbol *fn_symbol() const;

        void set_fn_symbol(Core::FunctionSymbol *fn_symbol);

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
