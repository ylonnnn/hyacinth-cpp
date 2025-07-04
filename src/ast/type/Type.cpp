#include "ast/type/Type.hpp"

namespace AST
{
    Type::Type(::Program::Position position) : Node(std::move(position)) {}

} // namespace AST
