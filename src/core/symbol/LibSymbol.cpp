#include "core/symbol/LibSymbol.hpp"
#include "core/symbol/Symbol.hpp"

namespace Core
{
    LibSymbol::LibSymbol(std::string_view name,
                         SymbolAccessibility accessibility,
                         Core::Position &declared_at, AST::LibBlock *node)
        : Symbol(name, accessibility, declared_at, node)
    {
        if (node != nullptr) [[unlikely]]
            define(&declared_at);
    }

    void LibSymbol::define(Core::Position *position) { defined_at = position; }

    void LibSymbol::print(std::ostream &os, uint8_t) const
    {
        os << "(lib) " << name << " { "
           << (accessibility == SymbolAccessibility::Public ? "Public"
                                                            : "Private")
           << " }";
    }

} // namespace Core
