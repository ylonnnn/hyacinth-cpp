#pragma once

#include <cstdint>
#include <ostream>

#include "core/program/Program.hpp"
#include "core/value/Value.hpp"

namespace AST
{
    class Node
    {
      protected:
        Core::Position *position_ = nullptr, *end_position_ = nullptr;
        std::shared_ptr<Core::Value> value_;

      public:
        Node(Core::Position &position);
        virtual ~Node();

      public:
        Core::ProgramFile &program();
        const Core::ProgramFile &program() const;
        Core::Position &position();
        const Core::Position &position() const;
        Core::Position &end_position();
        const Core::Position &end_position() const;

        Core::Value *value();
        const Core::Value *value() const;

        void set_value(std::shared_ptr<Core::Value> value);

        std::shared_ptr<Core::Value> &value_ptr();
        const std::shared_ptr<Core::Value> &value_ptr() const;

        void set_position(const Core::Position &position);
        void set_end_position(const Core::Position &position);

        virtual void print(std::ostream &os, uint8_t tab) const = 0;

        friend std::ostream &operator<<(std::ostream &os, const Node &node);
    };

} // namespace AST
