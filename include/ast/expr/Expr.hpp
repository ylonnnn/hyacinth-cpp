#pragma once

#include <cstdint>

#include "ast/Node.hpp"
#include "core/program/Program.hpp"

namespace AST
{
    class Expr : public Node
    {
      public:
        Expr(Core::Position program);
        virtual ~Expr() = default;

      public:
        virtual void print(std::ostream &os, uint8_t tab) const = 0;
    };

} // namespace AST
