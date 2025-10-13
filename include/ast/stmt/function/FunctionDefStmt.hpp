#pragma once

#include "ast/stmt/BlockStmt.hpp"
#include "ast/stmt/function/FunctionDeclStmt.hpp"

namespace AST
{
    struct FunctionDefinitionStmt : FunctionDeclarationStmt
    {
        FunctionDefinitionStmt(
            Lexer::Token &identifier, std::unique_ptr<Type> &&return_type,
            std::vector<std::unique_ptr<FunctionParameter>> &&parameters,
            std::unique_ptr<BlockStmt> &&body,
            DeclarationAccessibility accessibility =
                DeclarationAccessibility::Private);

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
