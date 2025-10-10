#include "ast/stmt/function/FunctionDefStmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    FunctionDefinitionStmt::FunctionDefinitionStmt(
        Lexer::Token &name, std::vector<FunctionParameter> parameters,
        std::unique_ptr<Block> body, DeclarationAccessibility accessibility)
        : Node(name.range.start),
          FunctionDeclarationStmt(name, std::move(parameters), accessibility)
    {
        body = std::move(body);

        end_position = body->end_position;
    }

    void FunctionDefinitionStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "FunctionDefinitionStmt {\n"
           << inner_indentation << "accessibility: " << accessibility << "\n"
           << inner_indentation << "name: " << name << "\n";
        // << inner_indentation << "return_type: ";

        // if (return_type_ != nullptr)
        //     return_type_->print(os, tab + 1);
        // else
        //     os << "nullptr";

        os << "\n" << inner_indentation << "parameters: {";
        for (auto &param : parameters)
        {
            std::string inner_indentation = utils::tab(tab + 1, 4);

            os << "\n" << inner_indentation;
            param.print(os, tab + 2);
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
