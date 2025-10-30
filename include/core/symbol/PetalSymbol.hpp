#pragma once

#include "ast/stmt/petal/PetalDeclStmt.hpp"
#include "ast/stmt/petal/PetalDefStmt.hpp"
#include "core/environment/PetalEnvironment.hpp"
#include "core/symbol/Symbol.hpp"

namespace Core
{
    struct PetalSymbol : Symbol
    {
        AST::PetalDeclarationStmt *decl_node = nullptr;
        AST::PetalDefinitionStmt *def_node = nullptr;

        Core::PetalEnvironment *environment = nullptr;

        PetalSymbol(std::string_view name, SymbolAccessibility accessibility,
                    AST::PetalDeclarationStmt *decl_node,
                    Core::PetalEnvironment *environment);

        void define(AST::Node *node) override;
        void define(AST::PetalDefinitionStmt *node);

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace Core
