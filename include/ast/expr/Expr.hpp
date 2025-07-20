#pragma once

#include <cstdint>

#include "ast/Node.hpp"

namespace AST
{
    class Expr : virtual public Node
    {
      public:
        Expr() = default;
        virtual ~Expr() = default;

      public:
        virtual void print(std::ostream &os, uint8_t tab) const = 0;
    };

} // namespace AST
