#include "ast/expr/LiteralExpr.hpp"
#include "ast/expr/Expr.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    LiteralExpr::LiteralExpr(Lexer::Token &value)
        : Expr(value.position), value_(value)
    {
        end_pos_ = value_.position.col + value_.value.size();
    }

    Lexer::Token &LiteralExpr::value() { return value_; }

    void LiteralExpr::print(std::ostream &os, uint8_t tab) const
    {
        os << "LiteralExpr { value: " << value_ << " }";
    }

} // namespace AST
