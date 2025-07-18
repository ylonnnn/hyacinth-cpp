#include "ast/stmt/Stmt.hpp"

namespace AST
{
    Stmt::Stmt(Core::Position position) : Node(std::move(position)) {}

} // namespace AST
