#include "ast/expr/Expr.hpp"
#include "ast/Node.hpp"
#include "program/Program.hpp"

namespace AST
{
    Expr::Expr(Program::Position &&position) : Node(std::move(position)) {}

} // namespace AST
