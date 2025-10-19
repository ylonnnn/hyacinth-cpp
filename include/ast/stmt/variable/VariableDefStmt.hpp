#pragma once

#include "ast/expr/Expr.hpp"
#include "ast/stmt/variable/VariableDeclStmt.hpp"

namespace AST
{
    struct VariableDefinitionStmt : VariableDeclarationStmt
    {
        std::unique_ptr<Expr> value;

        VariableDefinitionStmt(
            Lexer::Token &name, IdentifierMutabilityState mut_state,
            std::unique_ptr<Type> type, std::unique_ptr<Expr> value,
            DeclarationAccessibility DeclarationAccessibility =
                DeclarationAccessibility::Private);

        void print(std::ostream &os, uint32_t tab) const override;
    };

} // namespace AST
