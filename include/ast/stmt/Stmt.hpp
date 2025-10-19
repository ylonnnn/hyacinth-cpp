#pragma once

#include <cstdint>
#include <iostream>

#include "ast/Node.hpp"

namespace AST
{
    struct Stmt : virtual Node
    {
        Stmt() = default;
        virtual ~Stmt() = default;

        virtual void print(std::ostream &os, uint32_t tab) const = 0;
    };

} // namespace AST
