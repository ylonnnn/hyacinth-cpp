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

        VariableSymbol(std::string_view name, Core::Position declared_at,
                       bool is_mutable, std::unique_ptr<Type> type,
                       std::optional<Value> value = std::nullopt,
                       AST::VariableDeclarationStmt *node = nullptr);
    };

} // namespace Core
