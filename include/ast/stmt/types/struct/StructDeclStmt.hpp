#pragma once

#include "ast/stmt/DeclarationStmt.hpp"

namespace AST
{
    struct StructDeclarationStmt : DeclarationStmt
    {
        StructDeclarationStmt(Lexer::Token &identifier,
                              DeclarationAccessibility accessibility =
                                  DeclarationAccessibility::Private);

        void print(std::ostream &os, uint32_t tab) const override;
    };

} // namespace AST
