#include "core/symbol/PetalSymbol.hpp"
#include "utils/dev.hpp"

namespace Core
{
    PetalSymbol::PetalSymbol(std::string_view name,
                             SymbolAccessibility accessibility,
                             AST::PetalDeclarationStmt *decl_node,
                             Core::PetalEnvironment *environment)
        : Symbol(name, accessibility, nullptr), decl_node(decl_node),
          environment(environment)
    {
    }

    void PetalSymbol::define(AST::Node *node)
    {
        if (auto ptr = dynamic_cast<AST::PetalDefinitionStmt *>(node))
            def_node = ptr;
    }

    void PetalSymbol::define(AST::PetalDefinitionStmt *node)
    {
        def_node = node;
    }

    void PetalSymbol::print(std::ostream &os, uint8_t tab) const
    {
        utils::todo("implement PetalSymbol::print()");
    }

} // namespace Core
