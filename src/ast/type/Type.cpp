#include "ast/type/Type.hpp"

namespace AST
{
    Type::Type(Lexer::Token &value) : Node(value.position), value_(value)
    {
        set_end_position(value.end_position);
    }

    Lexer::Token &Type::value() { return value_; }

    const Lexer::Token &Type::value() const { return value_; }

} // namespace AST
