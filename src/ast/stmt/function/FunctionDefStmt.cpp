#include "ast/stmt/function/FunctionDefStmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    FunctionDefinitionStmt::FunctionDefinitionStmt(
        Lexer::Token &name, std::unique_ptr<Type> return_type,
        std::vector<FunctionParameter> parameters,
        std::unique_ptr<BlockStmt> body)
        : Node(name.position),
          FunctionDeclarationStmt(name, std::move(return_type),
                                  std::move(parameters))
    {
        body_ = std::move(body);

        end_pos_ = body_->end_pos();
    }

    bool FunctionDefinitionStmt::is_definition() const { return true; }

    BlockStmt &FunctionDefinitionStmt::body() { return *body_; }

    std::unique_ptr<BlockStmt> &FunctionDefinitionStmt::body_ptr()
    {
        return body_;
    }

    void FunctionDefinitionStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "FunctionDefinitionStmt {\n"
           << inner_indentation << "name: " << name_ << "\n"
           << inner_indentation << "return_type: ";

        return_type_->print(os, tab + 1);

        os << "\n" << inner_indentation << "parameters: {";
        for (auto &param : parameters_)
        {
            std::string inner_indentation = Utils::tab(tab + 1, 4);

            os << "\n" << inner_indentation;
            param.print(os, tab + 2);
        }

        os << "\n"
           << inner_indentation << "}\n"
           << inner_indentation << "body: {";
        for (auto &stmt : body_->statements())
        {
            std::string inner_indentation = Utils::tab(tab + 1, 4);

            os << "\n" << inner_indentation;
            stmt->print(os, tab + 2);
        }

        os << "\n" << inner_indentation << "}\n" << indentation << "}";
    }

} // namespace AST
