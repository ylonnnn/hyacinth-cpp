#include "ast/stmt/types/struct/StructDeclStmt.hpp"
#include "ast/stmt/DeclarationStmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    StructDeclarationStmt::StructDeclarationStmt(
        Lexer::Token &identifier, DeclarationAccessibility accessibility)
        : Node(identifier.range.start()),
          DeclarationStmt(identifier, accessibility, false)
    {
        range.end(identifier.range.end());
    }

    void StructDeclarationStmt::print(std::ostream &os, uint32_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "StructDeclarationStmt {\n"
           << inner_indentation << "accessibility: " << accessibility << "\n"
           << inner_indentation << "name: " << identifier << "\n"
           << indentation << "}";
    }

} // namespace AST
