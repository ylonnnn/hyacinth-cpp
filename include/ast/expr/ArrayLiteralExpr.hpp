#pragma once

#include <vector>

#include "ast/expr/Expr.hpp"
#include "ast/type/PrefixedType.hpp"

namespace AST
{
    struct ArrayLiteralExpr : Expr
    {
        std::unique_ptr<PrefixedType> type;
        std::vector<std::unique_ptr<Expr>> elements;

        ArrayLiteralExpr(Core::Position &position,
                         std::unique_ptr<PrefixedType> &&type = nullptr);

        void print(std::ostream &os, uint32_t tab) const override;
    };

} // namespace AST
