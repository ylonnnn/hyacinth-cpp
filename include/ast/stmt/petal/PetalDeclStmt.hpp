#pragma once

#include "ast/expr/Path.hpp"
#include "ast/stmt/DeclarationStmt.hpp"

namespace AST
{
    struct PetalDeclarationStmt : DeclarationStmt
    {
        std::unique_ptr<Path> identifier;

        PetalDeclarationStmt(std::unique_ptr<Path> &&identifier,
                             DeclarationAccessibility accessibility =
                                 DeclarationAccessibility::Private);

        void print(std::ostream &os, uint32_t tab) const override;
    };

} // namespace AST
