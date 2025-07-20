#pragma once

#include <cstdint>
#include <iostream>

#include "ast/Node.hpp"

namespace AST
{
    class Stmt : virtual public Node
    {
      public:
        Stmt() = default;
        virtual ~Stmt() = default;

      public:
        virtual void print(std::ostream &os, uint8_t tab) const = 0;
    };

} // namespace AST
