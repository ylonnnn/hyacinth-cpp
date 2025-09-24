#include "ast/stmt/types/struct/StructDeclStmt.hpp"
#include "ast/stmt/DeclarationStmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    StructDeclarationStmt::StructDeclarationStmt(Lexer::Token &name)
        : Node(name.position), DeclarationStmt(name)
    {
        set_end_position(name_.end_position);
    }

    bool StructDeclarationStmt::is_definition() const { return false; }

    void StructDeclarationStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "StructDeclarationStmt {\n"
           << inner_indentation << "accessibility: " << accessibility_ << "\n"
           << inner_indentation << "name: " << name_.value << "\n"
           << indentation << "}";
    }

} // namespace AST
