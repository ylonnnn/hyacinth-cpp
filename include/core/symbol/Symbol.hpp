#pragma once

#include "ast/Node.hpp"

namespace Core
{
    enum class SymbolAccessibility
    {
        Public = 0,
        Private,
    };

    struct Symbol
    {
        std::string_view name;
        AST::Node *decl_node = nullptr;
        AST::Node *def_node = nullptr;

        SymbolAccessibility accessibility;

        Symbol(std::string_view name, SymbolAccessibility accessibility,
               AST::Node *decl_node = nullptr);
        virtual ~Symbol() = default;

        virtual void define(AST::Node *node);

        virtual void print(std::ostream &os, uint8_t tab) const = 0;
    };

} // namespace Core
