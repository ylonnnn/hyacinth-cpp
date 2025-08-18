#pragma once

#include <vector>

#include "ast/expr/Expr.hpp"

namespace AST
{
    class ArrayExpr : public Expr
    {
      private:
        std::vector<std::unique_ptr<Expr>> elements_;

      public:
        ArrayExpr(Core::Position &position,
                  std::vector<std::unique_ptr<Expr>> elements);

        std::vector<std::unique_ptr<Expr>> &elements();
        const std::vector<std::unique_ptr<Expr>> &elements() const;

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
