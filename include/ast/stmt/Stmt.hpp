#pragma once

#include <cstdint>
#include <iostream>

#include "ast/Node.hpp"
#include "program/Program.hpp"

namespace AST
{
    class Stmt : public Node
    {
      public:
        Stmt(::Program::Position position);
        virtual ~Stmt() = default;

      public:
        virtual void print(std::ostream &os, uint8_t tab) const = 0;
    };

} // namespace AST
