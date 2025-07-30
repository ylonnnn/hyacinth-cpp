#pragma once

#include "ast/expr/Expr.hpp"
#include <vector>

namespace AST
{
    class FunctionCalLExpr : public Expr
    {
      private:
        std::unique_ptr<Expr> callee_;
        std::vector<std::unique_ptr<Expr>> arguments_;

      public:
        FunctionCalLExpr(std::unique_ptr<Expr> callee,
                         std::vector<std::unique_ptr<Expr>> arguments);

        Expr &callee();
        std::vector<std::unique_ptr<Expr>> &arguments();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
