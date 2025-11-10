#pragma once

#include "ast/expr/Expr.hpp"
#include "ast/stmt/variable/VariableDeclStmt.hpp"

namespace AST
{
    struct VariableDefinitionStmt : VariableDeclarationStmt
    {
        bool is_constexpr = false;
        std::unique_ptr<Expr> value;

        VariableDefinitionStmt(
            Lexer::Token &name, bool is_constexpr,
            IdentifierMutabilityState mut_state, std::unique_ptr<Type> type,
            std::unique_ptr<Expr> value,
            DeclarationAccessibility DeclarationAccessibility =
                DeclarationAccessibility::Private);

        void print(std::ostream &os, uint32_t tab) const override;
    };

} // namespace AST
