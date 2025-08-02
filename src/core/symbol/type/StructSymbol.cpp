#include "core/symbol/type/StructSymbol.hpp"
#include "core/symbol/Symbol.hpp"

namespace Core
{
    StructSymbol::StructSymbol(std::string_view name,
                               SymbolAccessibility accessibility,
                               Core::Position &declared_at,
                               AST::StructDeclarationStmt *node)
        : TypeSymbol(name, accessibility, declared_at, node)
    {
        this->node = dynamic_cast<AST::StructDeclarationStmt *>(node);

        if (typeid(*node) == typeid(AST::StructDefinitionStmt))
            define(static_cast<AST::StructDefinitionStmt *>(node));
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
