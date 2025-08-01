#include "core/symbol/type/StructSymbol.hpp"
#include "core/symbol/Symbol.hpp"

namespace Core
{
    StructSymbol::StructSymbol(std::string_view name,
                               SymbolAccessibility accessibility,
                               Core::Position declared_at,
                               AST::StructDeclarationStmt *node)
        : Symbol(name, accessibility, std::move(declared_at), node)
    {
        this->node = dynamic_cast<AST::StructDeclarationStmt *>(Symbol::node);
    }

    void StructSymbol::define(Core::Position *position)
    {
        defined_at = position;
    }

    void StructSymbol::define(AST::StructDefinitionStmt *definition)
    {
        define(&definition->position());
        this->definition = definition;
    }

} // namespace Core
