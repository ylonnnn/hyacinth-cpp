#include "ast/stmt/petal/PetalDeclStmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    PetalDeclarationStmt::PetalDeclarationStmt(
        std::unique_ptr<Path> &&identifier,
        DeclarationAccessibility accessibility)
        : Node(identifier->position),
          DeclarationStmt(identifier->segments.front()->identifier,
                          accessibility, false),
          identifier(std::move(identifier))
    {
    }

    void PetalDeclarationStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "PetalDeclarationStmt {\n" << inner_indentation << "identifier: ";
        identifier->print(os, tab + 1);

        os << inner_indentation << "accessibility: " << accessibility << "\n"
           << indentation << "}";
    }

} // namespace AST
