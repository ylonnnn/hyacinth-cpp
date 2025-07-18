#include "ast/stmt/VariableDefStmt.hpp"
#include "ast/stmt/VariableDeclStmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    VariableDefinitionStmt::VariableDefinitionStmt(
        Lexer::Token &name, VariableMutabilityState mut_state,
        std::unique_ptr<Type> type, std::unique_ptr<Expr> value)
        : VariableDeclarationStmt(name, mut_state, std::move(type)),
          value_(std::move(value))
    {
        end_pos_ = value_->end_pos();
    }

    bool VariableDefinitionStmt::is_definition() const { return true; }

    Expr &VariableDefinitionStmt::value() { return *value_; }

    std::unique_ptr<Expr> &VariableDefinitionStmt::value_ptr()
    {
        return value_;
    }

    void VariableDefinitionStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "VariableDefinitionStmt {"
           << "\n"
           << inner_indentation << "name: " << name_ << "\n"
           << inner_indentation << "mut_state: "
           << (mut_state_ == VariableMutabilityState::Mutable ? "Mutable"
                                                              : "Immutable")
           << "\n"
           << inner_indentation << "type: ";

        type_->print(os, tab + 1);

        os << "\n" << inner_indentation << "value: ";

        value_->print(os, tab + 1);

        os << "\n" << indentation << "}";
    }

} // namespace AST
