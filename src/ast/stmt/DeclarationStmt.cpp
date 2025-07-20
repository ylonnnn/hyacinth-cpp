#include "ast/stmt/DeclarationStmt.hpp"

namespace AST
{
    DeclarationStmt::DeclarationStmt(Lexer::Token &name)
        : Node(name.position), name_(name)
    {
    }

    Lexer::Token &DeclarationStmt::name() { return name_; }

} // namespace AST
