#include "ast/stmt/DeclarationStmt.hpp"

namespace AST
{
    DeclarationStmt::DeclarationStmt(Lexer::Token &name)
        : Node(name.position), name_(name)
    {
    }

    Lexer::Token &DeclarationStmt::name() { return name_; }

    DeclarationAccessibility DeclarationStmt::accessibility() const
    {
        return accessibility_;
    }

    void
    DeclarationStmt::set_accessibility(DeclarationAccessibility accessibility)
    {
        accessibility_ = accessibility;
    }

} // namespace AST
