#pragma once

#include "ast/common/IdentifierDecl.hpp"
#include "ast/expr/Expr.hpp"
#include "ast/stmt/BlockStmt.hpp"
#include "ast/stmt/DeclarationStmt.hpp"

namespace AST
{
    struct FunctionParameter : IdentifierDecl
    {
        std::unique_ptr<Expr> default_value;

        FunctionParameter(Lexer::Token &identifier,
                          IdentifierMutabilityState mut_state,
                          std::unique_ptr<Type> &&type,
                          std::unique_ptr<Expr> &&default_value = nullptr);

        void print(std::ostream &os, uint8_t tab) const override;
    };

    struct FunctionDeclarationStmt : DeclarationStmt
    {
        std::unique_ptr<Type> return_type;
        std::vector<std::unique_ptr<FunctionParameter>> parameters;

        // Empty
        std::unique_ptr<BlockStmt> body;

        FunctionDeclarationStmt(
            Lexer::Token &name, std::unique_ptr<Type> &&return_type,
            std::vector<std::unique_ptr<FunctionParameter>> &&parameters,
            DeclarationAccessibility accessibility =
                DeclarationAccessibility::Private);

        virtual void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
