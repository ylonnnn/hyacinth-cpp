#include "ast/stmt/variable/VariableDeclStmt.hpp"
#include "ast/Node.hpp"
#include "ast/common/Identifier.hpp"
#include "utils/style.hpp"

namespace AST
{
    VariableIdentifier::VariableIdentifier(Lexer::Token &name,
                                           IdentifierMutabilityState mut_state,
                                           std::unique_ptr<Type> type)
        : Identifier(name, mut_state, std::move(type))
    {
    }

    VariableDeclarationStmt::VariableDeclarationStmt(
        Lexer::Token &name, IdentifierMutabilityState mut_state,
        std::unique_ptr<Type> type)
        : Node(name.position), DeclarationStmt(name),
          VariableIdentifier(name, mut_state, std::move(type)), name_(name)
    {
        set_end_position(*VariableIdentifier::end_position_);
    }

    bool VariableDeclarationStmt::is_definition() const { return false; }

    Lexer::Token &VariableDeclarationStmt::name() { return name_; }

    const Lexer::Token &VariableDeclarationStmt::name() const { return name_; }

    Type *VariableDeclarationStmt::type() { return type_.get(); }

    const Type *VariableDeclarationStmt::type() const { return type_.get(); }

    void VariableDeclarationStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "VariableDeclarationStmt {"
           << "\n"
           << inner_indentation << "accessibility: " << accessibility_ << "\n"
           << inner_indentation << "name: " << name_ << "\n"
           << inner_indentation << "mut_state: "
           << (mut_state_ == IdentifierMutabilityState::Mutable ? "Mutable"
                                                                : "Immutable")
           << "\n"
           << inner_indentation << "type: ";

        if (type_ != nullptr)
            type_->print(os, tab + 1);

        os << "\n" << indentation << "}";
    }

} // namespace AST
