#pragma once

#include <cstdint>
#include <ostream>

#include "program/Program.hpp"

namespace AST
{
    class Node
    {
      protected:
        Program::Position position_;
        size_t end_pos_;
 
      public:
        Node(Program::Position &&position);
        virtual ~Node() = default;

      public:
        Program::ProgramFile &program();
        const Program::Position &position() const;
        virtual size_t end_pos(); 

        virtual void print(std::ostream &os, uint8_t tab) const = 0;

        friend std::ostream &operator<<(std::ostream &os, const Node &node);
    };

} // namespace AST
