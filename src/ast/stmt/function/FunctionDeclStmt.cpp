#include "ast/stmt/function/FunctionDeclStmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    FunctionParameter::FunctionParameter(Lexer::Token &identifier,
                                         IdentifierMutabilityState mut_state,
                                         std::unique_ptr<Type> &&type,
                                         std::unique_ptr<Expr> &&default_value)
        : Node(identifier.range.start),
          IdentifierDecl(identifier, mut_state, std::move(type)),
          default_value(std::move(default_value))
    {
    }

    void FunctionParameter::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "FunctionParameter {"
           << "\n"
           << inner_indentation << "identifier: " << identifier << "\n"
           << inner_indentation << "mut_state: "
           << (mut_state == IdentifierMutabilityState::Mutable ? "Mutable"
                                                               : "Immutable")
           << "\n"
           << inner_indentation << "type: ";

        type->print(os, tab + 1);

        os << "\n" << inner_indentation << "default_value: ";

        if (default_value != nullptr)
            default_value->print(os, tab + 1);
        else
            os << "<None>";

        os << "\n" << indentation << "}";
    }

    FunctionDeclarationStmt::FunctionDeclarationStmt(
        Lexer::Token &name, std::unique_ptr<Type> &&return_type,
        std::vector<std::unique_ptr<FunctionParameter>> &&parameters,
        DeclarationAccessibility accessibility)
        : Node(name.range.start), DeclarationStmt(name, accessibility, false),
          return_type(std::move(return_type)), parameters(std::move(parameters))
    {
        if (this->return_type != nullptr)
            end_position = this->parameters.empty()
                               ? this->return_type->end_position
                               : this->parameters.back()->end_position;
    }

    void FunctionDeclarationStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "FunctionDeclarationStmt {\n"
           << inner_indentation << "accessibility: " << accessibility << "\n"
           << inner_indentation << "identifier: " << identifier << "\n"
           << inner_indentation << "return_type: ";

        if (return_type != nullptr)
            return_type->print(os, tab + 1);
        else
            os << "nullptr";

        os << "\n" << inner_indentation << "parameters: {";

        for (const auto &param : parameters)
        {
            std::string inner_indentation = utils::tab(tab + 1, 4);

            os << "\n" << inner_indentation;
            param->print(os, tab + 2);
        }

        os << "\n" << inner_indentation << "}\n" << indentation << "}";
    }

} // namespace AST
