#pragma once

#include "ast/stmt/BlockStmt.hpp"
#include "ast/stmt/function/FunctionDeclStmt.hpp"

namespace AST
{
    class FunctionDefinitionStmt : public FunctionDeclarationStmt
    {
      public:
        FunctionDefinitionStmt(
            Lexer::Token &name, std::unique_ptr<Type> return_type,
            std::unique_ptr<NodeCollection<FunctionParameterIdentifier>>
                parameters,
            std::unique_ptr<BlockStmt> body);

        BlockStmt &body();

        std::unique_ptr<BlockStmt> &body_ptr();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
