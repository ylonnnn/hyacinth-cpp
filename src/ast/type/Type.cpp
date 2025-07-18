#include "ast/type/Type.hpp"

namespace AST
{
    Type::Type(Lexer::Token &value)
        : Node(value.position), value_(value)
    {
    }

    Lexer::Token &Type::value() { return value_; }

} // namespace AST
