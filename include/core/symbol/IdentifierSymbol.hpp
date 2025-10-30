#pragma once

#include "core/symbol/Symbol.hpp"
#include "core/type/Type.hpp"

namespace Core
{
    struct IdentifierSymbol : Symbol
    {
        bool is_mutable;
        InstantiatedType *type = nullptr;
        Value *value = nullptr;

        IdentifierSymbol(std::string_view name, AST::Node *decl_node,
                         bool is_mutable, InstantiatedType *type,
                         Value *value = nullptr);

        virtual void define(AST::Node *node) override = 0;

        virtual void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace Core
