#pragma once

#include <cstdint>
#include <ostream>

#include "core/program/Program.hpp"

namespace AST
{
    class Node
    {
      protected:
        Core::Position position_;
        size_t end_pos_;

      public:
        Node(Core::Position position);
        virtual ~Node() = default;

      public:
        Core::ProgramFile &program();
        const Core::Position &position() const;
        virtual size_t end_pos() const;

        virtual void print(std::ostream &os, uint8_t tab) const = 0;

        friend std::ostream &operator<<(std::ostream &os, const Node &node);
    };

} // namespace AST
