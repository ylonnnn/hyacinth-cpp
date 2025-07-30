#pragma once

#include <vector>

#include "ast/NodeCollection.hpp"
#include "ast/expr/Expr.hpp"
#include "core/program/Program.hpp"

namespace AST
{
    class ArrayExpr : public NodeCollection<Expr>
    {
      private:
        std::vector<std::unique_ptr<Expr>> elements_;

      public:
        ArrayExpr(Core::Position &position,
                  std::vector<std::unique_ptr<Expr>> collection);

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
