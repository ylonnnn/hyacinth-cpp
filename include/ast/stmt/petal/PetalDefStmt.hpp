#pragma once

#include "ast/stmt/BlockStmt.hpp"
#include "ast/stmt/petal/PetalDeclStmt.hpp"

namespace AST
{
    struct PetalDefinitionStmt : PetalDeclarationStmt
    {
        std::unique_ptr<BlockStmt> block;

        PetalDefinitionStmt(std::unique_ptr<Path> &&identifier,
                            std::unique_ptr<BlockStmt> &&block,
                            DeclarationAccessibility accessibility =
                                DeclarationAccessibility::Private);

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
