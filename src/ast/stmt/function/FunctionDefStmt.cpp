#include "ast/stmt/function/FunctionDefStmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    FunctionDefinitionStmt::FunctionDefinitionStmt(
        Lexer::Token &identifier, std::unique_ptr<Type> &&return_type,
        std::vector<std::unique_ptr<FunctionParameter>> &&parameters,
        std::unique_ptr<BlockStmt> &&body,
        DeclarationAccessibility accessibility)
        : Node(identifier.range.start),
          FunctionDeclarationStmt(identifier, std::move(return_type),
                                  std::move(parameters), accessibility)
    {
        this->body = std::move(body);
        end_position = this->body->end_position;
    }

    void FunctionDefinitionStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "FunctionDefinitionStmt {\n"
           << inner_indentation << "accessibility: " << accessibility << "\n"
           << inner_indentation << "identifier: " << identifier << "\n"
           << inner_indentation << "return_type: ";

        if (return_type != nullptr)
            return_type->print(os, tab + 1);
        else
            os << "<Deduced>";

        os << "\n" << inner_indentation << "parameters: {";
        for (auto &param : parameters)
        {
            std::string inner_indentation = utils::tab(tab + 1, 4);

            os << "\n" << inner_indentation;
            param->print(os, tab + 2);
        }

        os << "\n"
           << inner_indentation << "}\n"
           << inner_indentation << "body: {";
        for (auto &stmt : body->statements)
        {
            std::string inner_indentation = utils::tab(tab + 1, 4);

            os << "\n" << inner_indentation;
            stmt->print(os, tab + 2);
        }

        os << "\n" << inner_indentation << "}\n" << indentation << "}";
    }

} // namespace AST
