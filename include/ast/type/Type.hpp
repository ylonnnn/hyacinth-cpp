#pragma once

#include <cstdint>

#include "ast/Node.hpp"
#include "program/Program.hpp"

namespace AST
{
    class Type : public Node
    {
      public:
        Type(::Program::Position program);
        virtual ~Type() = default;

      public:
        virtual void print(std::ostream &os, uint8_t tab) const = 0;
    };

} // namespace AST
