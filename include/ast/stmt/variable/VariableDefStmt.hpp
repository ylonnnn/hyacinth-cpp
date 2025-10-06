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
            std::unique_ptr<Path> type, std::unique_ptr<Expr> value,
            DeclarationAccessibility DeclarationAccessibility =
                DeclarationAccessibility::Public);

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
