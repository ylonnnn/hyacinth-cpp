#pragma once

#include "ast/stmt/BlockStmt.hpp"
#include "ast/stmt/function/FunctionDeclStmt.hpp"

namespace AST
{
    class FunctionDefinitionStmt : public FunctionDeclarationStmt
    {
      public:
        FunctionDefinitionStmt(Lexer::Token &name,
                               std::unique_ptr<Type> return_type,
                               std::vector<FunctionParameter> parameters,
                               std::unique_ptr<BlockStmt> body);

        bool is_definition() const override;

        BlockStmt &body();

        std::unique_ptr<BlockStmt> &body_ptr();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
