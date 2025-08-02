#include "core/symbol/Symbol.hpp"

namespace Core
{
    Symbol::Symbol(std::string_view name, SymbolAccessibility accessibility,
                   Core::Position &declared_at, AST::Node *node)
        : name(std::move(name)), accessibility(accessibility),
          declared_at(declared_at), node(node)
    {
    }

    void Symbol::define(Core::Position *position) { defined_at = position; }

} // namespace Core
