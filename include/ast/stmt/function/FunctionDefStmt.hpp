#pragma once

#include "ast/block/Block.hpp"
#include "ast/stmt/function/FunctionDeclStmt.hpp"

namespace AST
{
    struct FunctionDefinitionStmt : FunctionDeclarationStmt
    {
        // TODO: Implement Function Return Type
        FunctionDefinitionStmt(Lexer::Token &name,
                               std::vector<FunctionParameter> parameters,
                               std::unique_ptr<Block> body,
                               DeclarationAccessibility accessibility =
                                   DeclarationAccessibility::Private);

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
