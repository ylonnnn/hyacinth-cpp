#pragma once

#include "ast/block/Block.hpp"
#include "ast/common/IdentifierDecl.hpp"
#include "ast/stmt/DeclarationStmt.hpp"
// #include "ast/type/Type.hpp"

namespace AST
{
    struct FunctionParameter : IdentifierDecl
    {
        // TODO: Default Value

        FunctionParameter(Lexer::Token &name,
                          IdentifierMutabilityState mut_state);
        // std::unique_ptr<Type> type);

        void print(std::ostream &os, uint8_t tab) const override;
    };

    struct FunctionDeclarationStmt : DeclarationStmt
    {
        // std::unique_ptr<Type> return_type_;
        std::vector<FunctionParameter> parameters;

        // Empty
        std::unique_ptr<Block> body;

        // TODO: Function Declaration Return Type
        FunctionDeclarationStmt(Lexer::Token &name,
                                std::vector<FunctionParameter> parameters,
                                DeclarationAccessibility accessibility =
                                    DeclarationAccessibility::Private);

        virtual void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
