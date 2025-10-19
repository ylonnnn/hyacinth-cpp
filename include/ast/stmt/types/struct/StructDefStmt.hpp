#pragma once

#include <unordered_map>

#include "ast/common/IdentifierDecl.hpp"
#include "ast/expr/Expr.hpp"
#include "ast/stmt/types/struct/StructDeclStmt.hpp"

namespace AST
{
    struct StructField : IdentifierDecl
    {
        std::unique_ptr<Expr> default_value;

        StructField(Lexer::Token &identifier,
                    IdentifierMutabilityState mut_state,
                    std::unique_ptr<Type> &&type,
                    std::unique_ptr<Expr> &&default_value = nullptr);

        void print(std::ostream &os, uint32_t tab) const override;
    };

    struct StructDefinitionStmt : StructDeclarationStmt
    {
        std::unordered_map<std::string_view, StructField> fields;

        StructDefinitionStmt(
            Lexer::Token &identifier,
            std::unordered_map<std::string_view, StructField> &&fields);

        void print(std::ostream &os, uint32_t tab) const override;
    };

} // namespace AST
