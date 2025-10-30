#pragma once

#include "core/symbol/Symbol.hpp"
#include "core/type/Type.hpp"

namespace Core
{
    struct TypeSymbol : Symbol
    {
        BaseType &base;

        TypeSymbol(std::string_view name, AST::Node *decl_node,
                   SymbolAccessibility accessibility, BaseType &base);

        void define(AST::Node *node) override;

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace Core
