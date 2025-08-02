#include <iostream>
#include <ostream>

#include "ast/Node.hpp"
#include "core/program/Program.hpp"

namespace AST
{
    Node::Node(Core::Position &position)
        : position_(&position), end_position_(position_)
    {
    }

    Node::~Node() = default;
    // Node::~Node() { std::cout << "destroyed\n"; }

    Core::ProgramFile &Node::program() { return position_->program; }

    Core::Position &Node::position() { return *position_; }

    Core::Position &Node::end_position() { return *end_position_; }

    void Node::set_position(const Core::Position &position)
    {
        position_ = const_cast<Core::Position *>(&position);
    }

    void Node::set_end_position(const Core::Position &position)
    {
        end_position_ = const_cast<Core::Position *>(&position);
    }

    std::ostream &operator<<(std::ostream &os, const Node &node)
    {
        node.print(os, 1);

        return os;
    }

} // namespace AST
