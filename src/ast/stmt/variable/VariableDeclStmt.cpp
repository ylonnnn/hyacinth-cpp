#include "ast/stmt/variable/VariableDeclStmt.hpp"
#include "ast/Node.hpp"
#include "utils/style.hpp"

namespace AST
{
    VariableDeclarationStmt::VariableDeclarationStmt(
        Lexer::Token &name, IdentifierMutabilityState mut_state,
        std::unique_ptr<Type> type, DeclarationAccessibility accessibility)
        : Node(name.range.start()), DeclarationStmt(name, accessibility, false),
          IdentifierDecl(name, mut_state, std::move(type)), name(name)
    {
        range.end(IdentifierDecl::range.end());

        if (this->type != nullptr)
            range.end(this->type->range.end());
    }

    void VariableDeclarationStmt::print(std::ostream &os, uint32_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "VariableDeclarationStmt {"
           << "\n"
           << inner_indentation << "accessibility: " << accessibility << "\n"
           << inner_indentation << "name: " << name << "\n"
           << inner_indentation << "mut_state: "
           << (mut_state == IdentifierMutabilityState::Mutable ? "Mutable"
                                                               : "Immutable")
           << "\n"
           << inner_indentation << "type: ";

        if (type != nullptr)
            type->print(os, tab + 1);
        else
            os << "<Deduced>";

        os << "\n" << indentation << "}";
    }

} // namespace AST
