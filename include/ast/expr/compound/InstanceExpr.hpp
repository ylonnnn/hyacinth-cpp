#pragma once

#include <unordered_map>

#include "ast/expr/Expr.hpp"

namespace AST
{
    class InstanceExpr : public Expr
    {
      private:
        std::unordered_map<std::string, std::unique_ptr<Expr>> fields_;

      public:
        InstanceExpr(
            Core::Position &position,
            std::unordered_map<std::string, std::unique_ptr<Expr>> fields);

        std::unordered_map<std::string, std::unique_ptr<Expr>> &fields();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
