#include "ast/stmt/Stmt.hpp"

namespace AST
{
    Stmt::Stmt(::Program::Position position) : Node(std::move(position)) {}

} // namespace AST
