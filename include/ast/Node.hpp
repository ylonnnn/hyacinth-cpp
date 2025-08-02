#pragma once

#include <cstdint>
#include <ostream>

#include "core/program/Program.hpp"

namespace AST
{
    class Node
    {
      protected:
        Core::Position *position_ = nullptr, *end_position_ = nullptr;

      public:
        Node(Core::Position &position);
        virtual ~Node();

      public:
        Core::ProgramFile &program();
        Core::Position &position();
        Core::Position &end_position();

        void set_position(const Core::Position &position);
        void set_end_position(const Core::Position &position);

        virtual void print(std::ostream &os, uint8_t tab) const = 0;

        friend std::ostream &operator<<(std::ostream &os, const Node &node);
    };

} // namespace AST
