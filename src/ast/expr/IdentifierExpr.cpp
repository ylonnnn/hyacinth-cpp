#include "ast/expr/IdentifierExpr.hpp"
#include "ast/expr/Expr.hpp"

namespace AST
{
    IdentifierExpr::IdentifierExpr(Lexer::Token &identifier)
        : Expr(::Program::Position(identifier.position)), identifier_(identifier)
    {
        end_pos_ = identifier_.position.col + identifier_.value.size();
    }

    Lexer::Token &IdentifierExpr::identifier() { return identifier_; }

    void IdentifierExpr::print(std::ostream &os, uint8_t tab) const
    {
        os << "IdentifierExpr { identifier: " << identifier_.value << " }";
    }

} // namespace AST
