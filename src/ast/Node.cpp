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

    const Core::ProgramFile &Node::program() const
    {
        return position_->program;
    }

    Core::Position &Node::position() { return *position_; }

    const Core::Position &Node::position() const { return *position_; }

    Core::Position &Node::end_position() { return *end_position_; }

    const Core::Position &Node::end_position() const { return *end_position_; }

    Core::Value *Node::value() { return value_.get(); }

    const Core::Value *Node::value() const { return value_.get(); }

    void Node::set_value(std::shared_ptr<Core::Value> value) { value_ = value; }

    std::shared_ptr<Core::Value> &Node::value_ptr() { return value_; }

    const std::shared_ptr<Core::Value> &Node::value_ptr() const
    {
        return value_;
    }

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
