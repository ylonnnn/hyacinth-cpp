#pragma once

#include "core/symbol/Symbol.hpp"
#include "core/type/Type.hpp"

namespace Core
{
    struct IdentifierSymbol : public Symbol
    {
        bool is_mutable;
        std::unique_ptr<Type> type;
        std::optional<Value> value = std::nullopt;

        IdentifierSymbol(std::string_view name, Core::Position declared_at,
                         bool is_mutable, std::unique_ptr<Type> type,
                         std::optional<Value> value, AST::Node *node = nullptr);
    };

} // namespace Core
