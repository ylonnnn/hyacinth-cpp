#pragma once

#include <cstdint>

#include "ast/Node.hpp"

namespace AST
{
    struct Expr : virtual Node
    {
        Expr() = default;
        virtual ~Expr() = default;

        virtual void print(std::ostream &os, uint8_t tab) const = 0;
    };

} // namespace AST
