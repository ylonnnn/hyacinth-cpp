#pragma once

#include "ast/stmt/variable/VariableDeclStmt.hpp"
#include "core/symbol/Symbol.hpp"
#include "core/type/Type.hpp"
#include "core/value/Value.hpp"

namespace Core
{
    struct VariableSymbol : public Symbol
    {
        AST::VariableDeclarationStmt *node = nullptr;

        bool is_mutable;
        Type *type;
        Value value;

        VariableSymbol(std::string name, Core::Position declared_at,
                       bool is_mutable, Type *type,
                       std::optional<Value> value = std::nullopt,
                       AST::VariableDeclarationStmt *node = nullptr);
    };

} // namespace Core
