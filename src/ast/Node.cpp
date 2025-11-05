#include <iostream>
#include <ostream>

#include "ast/Node.hpp"
#include "core/program/Program.hpp"

namespace AST
{
    Node::Node(Core::Position &position) : range{&position, &position} {}

    Node::~Node() = default;
    // Node::~Node() { std::cout << "destroyed\n"; }

    Core::Program &Node::program() { return range.start().program; }

    const Core::Program &Node::program() const { return range.start().program; }

    // void Node::set_value(std::shared_ptr<Core::Value> value) { value_ =
    // value; }

    std::ostream &operator<<(std::ostream &os, const Node &node)
    {
        node.print(os, 1);

        return os;
    }

} // namespace AST
