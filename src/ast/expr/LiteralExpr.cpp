#include "ast/expr/LiteralExpr.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    LiteralExpr::LiteralExpr(Lexer::Token &value)
        : Node(value.position), value_(value)
    {
        set_end_position(value_.end_position);
    }

    Lexer::Token &LiteralExpr::value() { return value_; }

    void LiteralExpr::print(std::ostream &os,
                            [[maybe_unused]] uint8_t tab) const
    {
        os << "LiteralExpr { value: " << value_ << " }";
    }

} // namespace AST
