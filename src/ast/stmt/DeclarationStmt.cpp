#include "ast/stmt/DeclarationStmt.hpp"
#include "ast/stmt/Stmt.hpp"

namespace AST
{
    DeclarationStmt::DeclarationStmt(Lexer::Token &name)
        : Stmt(name.position), name_(name)
    {
    }

    Lexer::Token &DeclarationStmt::name() { return name_; }

} // namespace AST
