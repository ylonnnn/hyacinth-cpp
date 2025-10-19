#include "ast/stmt/variable/VariableDefStmt.hpp"
#include "ast/stmt/variable/VariableDeclStmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    VariableDefinitionStmt::VariableDefinitionStmt(
        Lexer::Token &name, IdentifierMutabilityState mut_state,
        std::unique_ptr<Type> type, std::unique_ptr<Expr> value,
        DeclarationAccessibility accessibility)
        : Node(name.range.start),
          VariableDeclarationStmt(name, mut_state, std::move(type),
                                  accessibility),
          value(std::move(value))
    {
        is_definition_ = true;

        if (value != nullptr)
            end_position = value->end_position;
    }

    void VariableDefinitionStmt::print(std::ostream &os, uint32_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "VariableDefinitionStmt {"
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

        os << "\n" << inner_indentation << "value: ";

        value->print(os, tab + 1);

        os << "\n" << indentation << "}";
    }

} // namespace AST
