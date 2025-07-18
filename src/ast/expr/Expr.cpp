#include "ast/expr/Expr.hpp"
#include "ast/Node.hpp"
#include "core/program/Program.hpp"

namespace AST
{
    Expr::Expr(Core::Position position) : Node(std::move(position)) {}

} // namespace AST
