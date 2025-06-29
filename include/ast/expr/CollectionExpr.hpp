#pragma once

#include <vector>

#include "ast/NodeCollection.hpp"
#include "ast/expr/Expr.hpp"
#include "ast/expr/Expr.hpp"
#include "program/Program.hpp"

namespace AST
{
    class CollectionExpr : public NodeCollection<Expr>
    {
      public:
        CollectionExpr(::Program::Position position,
                       std::vector<std::unique_ptr<Expr>> collection);
        ~CollectionExpr() override = default;

      public:
        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
