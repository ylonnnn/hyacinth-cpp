#include "ast/stmt/types/struct/StructDeclStmt.hpp"
#include "ast/stmt/DeclarationStmt.hpp"

namespace AST
{
    StructDeclarationStmt::StructDeclarationStmt(Lexer::Token &name)
        : Node(name.position), DeclarationStmt(name)
    {
        end_pos_ = name.position.col + name.value.size();
    }

    bool StructDeclarationStmt::is_definition() const { return false; }

    void StructDeclarationStmt::print(std::ostream &os, uint8_t tab) const
    {
        os << "StructDeclarationStmt { name: " << name_.value << " }";
    }

} // namespace AST
