#pragma once

#include <cstdint>
#include <iostream>

#include "ast/common/Identifier.hpp"
#include "ast/expr/Expr.hpp"

namespace AST
{
    struct Path : Expr
    {
        std::vector<std::unique_ptr<Identifier>> segments;

        Path(std::vector<std::unique_ptr<Identifier>> &&segments);
        virtual ~Path() = default;

        void print(std::ostream &os, uint32_t tab) const override;
    };

} // namespace AST
