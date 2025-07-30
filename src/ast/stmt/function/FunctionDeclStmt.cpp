#include "ast/stmt/function/FunctionDeclStmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    FunctionParameter::FunctionParameter(Lexer::Token &name,
                                         IdentifierMutabilityState mut_state,
                                         std::unique_ptr<Type> type)
        : Node(name.position), Identifier(name, mut_state, std::move(type))
    {
    }

    void FunctionParameter::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "FunctionParameter {"
           << "\n"
           << inner_indentation << "name: " << name_ << "\n"
           << inner_indentation << "mut_state: "
           << (mut_state_ == IdentifierMutabilityState::Mutable ? "Mutable"
                                                                : "Immutable")
           << "\n"
           << inner_indentation << "type: ";

        type_->print(os, tab + 1);

        os << "\n" << indentation << "}";
    }

    FunctionDeclarationStmt::FunctionDeclarationStmt(
        Lexer::Token &name, std::unique_ptr<Type> return_type,
        std::vector<FunctionParameter> parameters)
        : Node(name.position), DeclarationStmt(name),
          return_type_(std::move(return_type)),
          parameters_(std::move(parameters))
    {
        if (return_type_ != nullptr)
            set_end_position(parameters_.empty()
                                 ? return_type_->end_position()
                                 : parameters_.back().end_position());
    }

    bool FunctionDeclarationStmt::is_definition() const { return false; }

    Type &FunctionDeclarationStmt::return_type() { return *return_type_; }

    std::vector<FunctionParameter> &FunctionDeclarationStmt::parameters()
    {
        return parameters_;
    }

    std::unique_ptr<Type> &FunctionDeclarationStmt::return_type_ptr()
    {
        return return_type_;
    }

    void FunctionDeclarationStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "FunctionDeclarationStmt {\n"
           << inner_indentation << "name: " << name_ << "\n"
           << inner_indentation << "return_type: ";

        if (return_type_ != nullptr)
            return_type_->print(os, tab + 1);
        else
            os << "nullptr";

        os << "\n" << inner_indentation << "parameters: {";

        for (const auto &param : parameters_)
        {
            std::string inner_indentation = Utils::tab(tab + 1, 4);

            os << "\n" << inner_indentation;
            param.print(os, tab + 2);
        }

        os << "\n" << inner_indentation << "}\n" << indentation << "}";
    }

} // namespace AST
