#pragma once

#include "ast/common/IdentifierDecl.hpp"
#include "ast/stmt/DeclarationStmt.hpp"
#include "lexer/Token.hpp"

namespace AST
{
    struct VariableDeclarationStmt : DeclarationStmt, IdentifierDecl
    {
        Lexer::Token &name;

        VariableDeclarationStmt(Lexer::Token &name,
                                IdentifierMutabilityState mut_state,
                                std::unique_ptr<Type> type,
                                DeclarationAccessibility accessibility =
                                    DeclarationAccessibility::Private);

        virtual void print(std::ostream &os, uint32_t tab) const override;
    };

} // namespace AST
