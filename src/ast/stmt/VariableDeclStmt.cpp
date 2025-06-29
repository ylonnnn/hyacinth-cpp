#include "ast/stmt/VariableDeclStmt.hpp"
#include "ast/stmt/Stmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    VariableDeclarationStmt::VariableDeclarationStmt(
        Lexer::Token &name, VariableMutabilityState mut_state)
        : Stmt(::Program::Position(name.position)), name_(name),
          mut_state_(mut_state)
    {
    }

    Lexer::Token &VariableDeclarationStmt::name() { return name_; }

    VariableMutabilityState VariableDeclarationStmt::mut_state() const
    {
        return mut_state_;
    }

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
           << indentation << "}";
    }

} // namespace AST
