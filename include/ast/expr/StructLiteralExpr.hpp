#pragma once

#include <unordered_map>

#include "ast/expr/Expr.hpp"
#include "ast/type/SimpleType.hpp"

namespace AST
{
    struct StructLiteralExpr : Expr
    {
        std::unique_ptr<SimpleType> type;
        std::unordered_map<std::string, std::unique_ptr<Expr>> fields;

        StructLiteralExpr(Core::Position &position,
                          std::unique_ptr<SimpleType> &&type = nullptr);

        void print(std::ostream &os, uint32_t tab) const override;
    };

} // namespace AST
