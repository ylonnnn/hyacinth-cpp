#pragma once

#include <vector>

#include "ast/expr/Expr.hpp"

namespace AST
{
    class FunctionCallExpr : public Expr
    {
      private:
        std::unique_ptr<Expr> callee_;
        std::vector<std::unique_ptr<Expr>> arguments_;

      public:
        FunctionCallExpr(std::unique_ptr<Expr> callee,
                         std::vector<std::unique_ptr<Expr>> arguments);

        Expr &callee();
        std::vector<std::unique_ptr<Expr>> &arguments();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
