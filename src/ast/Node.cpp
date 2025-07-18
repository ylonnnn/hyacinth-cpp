#include <ostream>

#include "ast/Node.hpp"
#include "core/program/Program.hpp"

namespace AST
{
    Node::Node(Core::Position position) : position_(std::move(position)) {}

    Core::ProgramFile &Node::program() { return position_.program; }

    const Core::Position &Node::position() const { return position_; }

    size_t Node::end_pos() const { return end_pos_; }

    std::ostream &operator<<(std::ostream &os, const Node &node)
    {
        node.print(os, 1);

        return os;
    }

} // namespace AST
