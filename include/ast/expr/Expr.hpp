#pragma once

#include <cstdint>

#include "ast/Node.hpp"
#include "program/Program.hpp"

namespace AST
{
    class Expr : public Node
    {
      public:
        Expr(::Program::Position program);
        virtual ~Expr() = default;

      public:
        virtual void print(std::ostream &os, uint8_t tab) const = 0;
    };

} // namespace AST
