#pragma once

#include <cstdint>
#include <ostream>

#include "core/program/Program.hpp"
#include "core/value/Value.hpp"

namespace AST
{
    struct Node
    {
        Core::Position &position, &end_position;
        std::shared_ptr<Core::Value> value_;

      public:
        Node(Core::Position &position);
        virtual ~Node();

      public:
        Core::ProgramFile &program();
        const Core::ProgramFile &program() const;

        void set_value(std::shared_ptr<Core::Value> value);

        virtual void print(std::ostream &os, uint8_t tab) const = 0;

        friend std::ostream &operator<<(std::ostream &os, const Node &node);
    };

} // namespace AST
