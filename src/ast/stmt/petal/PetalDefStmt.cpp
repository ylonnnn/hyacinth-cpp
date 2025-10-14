#include "ast/stmt/petal/PetalDefStmt.hpp"
#include "ast/stmt/petal/PetalDeclStmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    PetalDefinitionStmt::PetalDefinitionStmt(
        std::unique_ptr<Path> &&identifier, std::unique_ptr<BlockStmt> &&block,
        DeclarationAccessibility accessibility)
        : Node(identifier->position),
          PetalDeclarationStmt(std::move(identifier), accessibility),
          block(std::move(block))
    {
        is_definition_ = true;
    }

    void PetalDefinitionStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "PetalDefinitionStmt {\n" << inner_indentation << "identifier: ";
        identifier->print(os, tab + 1);

        os << "\n"
           << inner_indentation << "accessibility: " << accessibility << "\n"
           << inner_indentation << "block: ";
        block->print(os, tab + 1);

        os << "\n" << indentation << "}";
    }

} // namespace AST
