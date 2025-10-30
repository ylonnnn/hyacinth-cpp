#include "core/symbol/Symbol.hpp"

namespace Core
{
    Symbol::Symbol(std::string_view name, SymbolAccessibility accessibility,
                   AST::Node *decl_node)
        : name(name), accessibility(accessibility), decl_node(decl_node)
    {
    }

    void Symbol::define(AST::Node *node) { def_node = node; }

} // namespace Core
