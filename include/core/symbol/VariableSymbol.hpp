#pragma once

#include "ast/stmt/variable/VariableDeclStmt.hpp"
#include "core/symbol/IdentifierSymbol.hpp"
#include "core/type/Type.hpp"
#include "core/value/Value.hpp"

namespace Core
{
    struct VariableSymbol : public IdentifierSymbol
    {
        AST::VariableDeclarationStmt *node = nullptr;

        VariableSymbol(std::string_view name, SymbolAccessibility accessibility,
                       Core::Position declared_at, bool is_mutable, Type *type,
                       std::shared_ptr<Value> value = nullptr,
                       AST::VariableDeclarationStmt *node = nullptr);

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace Core
