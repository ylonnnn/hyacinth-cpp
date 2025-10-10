#include "ast/stmt/function/FunctionDeclStmt.hpp"
#include "utils/dev.hpp"
#include "utils/style.hpp"

namespace AST
{
    FunctionParameter::FunctionParameter(Lexer::Token &name,
                                         IdentifierMutabilityState mut_state)
        : Node(name.range.start), IdentifierDecl(name, mut_state, nullptr)
    {
        utils::todo("replace identifier path with valid type");
    }

    void FunctionParameter::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "FunctionParameter {"
           << "\n"
           << inner_indentation << "name: " << name << "\n"
           << inner_indentation << "mut_state: "
           << (mut_state == IdentifierMutabilityState::Mutable ? "Mutable"
                                                               : "Immutable")
           << "\n"
           << inner_indentation << "type: ";

        type->print(os, tab + 1);

        os << "\n" << indentation << "}";
    }

    FunctionDeclarationStmt::FunctionDeclarationStmt(
        Lexer::Token &name, std::vector<FunctionParameter> parameters,
        DeclarationAccessibility accessibility)
        : Node(name.range.start), DeclarationStmt(name, accessibility, false),
          parameters(std::move(parameters))
    {
        utils::todo("implement return type");
        // if (return_type_ != nullptr)
        //     set_end_position(parameters_.empty()
        //                          ? return_type_->end_position()
        //                          : parameters_.back().end_position());
    }

    void FunctionDeclarationStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "FunctionDeclarationStmt {\n"
           << inner_indentation << "accessibility: " << accessibility << "\n"
           << inner_indentation << "name: " << name << "\n";
        // << inner_indentation << "return_type: ";

        // if (return_type_ != nullptr)
        //     return_type_->print(os, tab + 1);
        // else
        //     os << "nullptr";

        os << "\n" << inner_indentation << "parameters: {";

        for (const auto &param : parameters)
        {
            std::string inner_indentation = utils::tab(tab + 1, 4);

            os << "\n" << inner_indentation;
            param.print(os, tab + 2);
        }

        os << "\n" << inner_indentation << "}\n" << indentation << "}";
    }

} // namespace AST
