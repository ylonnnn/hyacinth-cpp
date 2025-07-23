#include "ast/stmt/function/FunctionDeclStmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    FunctionParameterIdentifier::FunctionParameterIdentifier(
        Lexer::Token &name, IdentifierMutabilityState mut_state,
        std::unique_ptr<Type> type)
        : Node(name.position), Identifier(name, mut_state, std::move(type))
    {
    }

    void FunctionParameterIdentifier::print(std::ostream &os, uint8_t tab) const
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
        std::unique_ptr<NodeCollection<FunctionParameterIdentifier>> parameters)
        : Node(name.position), DeclarationStmt(name),
          return_type_(std::move(return_type)),
          parameters_(std::move(parameters))
    {
        auto &collection = parameters_->collection();
        end_pos_ = collection.empty() ? return_type_->end_pos()
                                      : collection.back()->end_pos();
    }

    bool FunctionDeclarationStmt::is_definition() const { return false; }

    void FunctionDeclarationStmt::set_end_pos(size_t end_pos)
    {
        end_pos_ = end_pos;
    }

    Type &FunctionDeclarationStmt::return_type() { return *return_type_; }

    std::vector<std::unique_ptr<FunctionParameterIdentifier>> &
    FunctionDeclarationStmt::parameters()
    {
        return parameters_->collection();
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

        return_type_->print(os, tab + 1);

        os << "\n" << inner_indentation << "parameters: {";

        for (auto &param : parameters_->collection())
        {
            std::string inner_indentation = Utils::tab(tab + 1, 4);

            os << "\n" << inner_indentation;
            param->print(os, tab + 2);
        }

        os << "\n" << inner_indentation << "}\n" << indentation << "}";
    }

} // namespace AST
