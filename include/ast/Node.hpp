#pragma once

#include <cstdint>
#include <ostream>

#include "core/position/Position.hpp"
#include "core/program/Program.hpp"
// #include "core/value/Value.hpp"

namespace AST
{
    struct Node
    {
        Core::Position &position, *end_position = nullptr;
        // std::shared_ptr<Core::Value> value_;

        Node(Core::Position &position);
        virtual ~Node();

        Core::Program &program();
        const Core::Program &program() const;

        // void set_value(std::shared_ptr<Core::Value> value);

        virtual void print(std::ostream &os, uint32_t tab) const = 0;

        friend std::ostream &operator<<(std::ostream &os, const Node &node);
    };

} // namespace AST
