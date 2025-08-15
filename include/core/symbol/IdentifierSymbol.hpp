#pragma once

#include "core/symbol/Symbol.hpp"
#include "core/type/Type.hpp"

namespace Core
{
    struct IdentifierSymbol : public Symbol
    {
        bool is_mutable;
        Type *type;
        std::shared_ptr<Value> value;

        IdentifierSymbol(std::string_view name, Core::Position declared_at,
                         bool is_mutable, Type *type,
                         std::shared_ptr<Value> value = nullptr,
                         AST::Node *node = nullptr);

        virtual void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace Core
