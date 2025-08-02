#include "core/symbol/type/TypeSymbol.hpp"

namespace Core
{
    TypeSymbol::TypeSymbol(std::string_view name,
                           SymbolAccessibility accessibility,
                           Core::Position &declared_at,
                           AST::DeclarationStmt *node)
        : Symbol(name, accessibility, declared_at, node)
    {
    }

} // namespace Core
