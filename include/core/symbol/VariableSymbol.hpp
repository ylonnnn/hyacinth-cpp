#pragma once

#include "ast/stmt/variable/VariableDeclStmt.hpp"
#include "core/symbol/IdentifierSymbol.hpp"
#include "core/type/Type.hpp"
#include "core/value/Value.hpp"

namespace Core
{
    struct VariableSymbol : IdentifierSymbol
    {
        AST::VariableDeclarationStmt *decl_node = nullptr;
        AST::Node *def_node = nullptr;

        VariableSymbol(std::string_view name, SymbolAccessibility acessibility,
                       AST::VariableDeclarationStmt *decl_node, bool is_mutable,
                       InstantiatedType *type, Value *value = nullptr);

        void define(AST::Node *node) override;

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace Core
