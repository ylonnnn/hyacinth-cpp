#include "ast/expr/IdentifierExpr.hpp"

namespace AST
{
    IdentifierExpr::IdentifierExpr(Lexer::Token &identifier)
        : Node(identifier.position), identifier_(identifier)
    {
        end_pos_ = identifier_.position.col + identifier_.value.size();
    }

    Lexer::Token &IdentifierExpr::identifier() { return identifier_; }

    void IdentifierExpr::print(std::ostream &os,
                               [[maybe_unused]] uint8_t tab) const
    {
        os << "IdentifierExpr { identifier: " << identifier_.value << " }";
    }

} // namespace AST
