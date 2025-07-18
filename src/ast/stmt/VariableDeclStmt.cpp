#include "ast/stmt/VariableDeclStmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    VariableDeclarationStmt::VariableDeclarationStmt(
        Lexer::Token &name, VariableMutabilityState mut_state,
        std::unique_ptr<Type> type)
        : DeclarationStmt(name), mut_state_(mut_state), type_(std::move(type))
    {
        end_pos_ = type_->end_pos();
    }

    bool VariableDeclarationStmt::is_definition() const { return false; }

    VariableMutabilityState VariableDeclarationStmt::mut_state() const
    {
        return mut_state_;
    }

    Type &VariableDeclarationStmt::type() { return *type_; }

    bool VariableDeclarationStmt::is_mutable() const
    {
        return mut_state_ == VariableMutabilityState::Mutable;
    }

    void VariableDeclarationStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "VariableDeclarationStmt {"
           << "\n"
           << inner_indentation << "name: " << name_ << "\n"
           << inner_indentation << "mut_state: "
           << (mut_state_ == VariableMutabilityState::Mutable ? "Mutable"
                                                              : "Immutable")
           << "\n"
           << inner_indentation << "type: ";

        type_->print(os, tab + 1);

        os << "\n" << indentation << "}";
    }

} // namespace AST
