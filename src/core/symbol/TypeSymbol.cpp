#include "core/symbol/TypeSymbol.hpp"
#include "core/symbol/Symbol.hpp"
#include "utils/dev.hpp"

namespace Core
{

    TypeSymbol::TypeSymbol(std::string_view name, AST::Node *decl_node,
                           SymbolAccessibility accessibility, BaseType &base)
        : Symbol(name, accessibility, decl_node), base(base)
    {
    }

    void TypeSymbol::define(AST::Node *node) { def_node = node; }

    void TypeSymbol::print(std::ostream &os, uint8_t tab) const
    {
        utils::todo("implement TypeSymbol::print()");
    }

} // namespace Core
