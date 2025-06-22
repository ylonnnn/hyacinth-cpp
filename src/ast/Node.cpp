#include <ostream>

#include "ast/Node.hpp"
#include "program/Program.hpp"

namespace AST
{
    Node::Node(Program::Position &&position) : position_(std::move(position)) {}

    Program::ProgramFile &Node::program() { return position_.program; }

    const Program::Position &Node::position() const { return position_; }

    size_t Node::end_pos() { return end_pos_; }

    std::ostream &operator<<(std::ostream &os, const Node &node)
    {
        node.print(os, 1);

        return os;
    }

} // namespace AST
