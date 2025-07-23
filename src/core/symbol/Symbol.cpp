#include "core/symbol/Symbol.hpp"

namespace Core
{
    Symbol::Symbol(std::string name, Core::Position declared_at,
                   AST::Node *node)
        : name(std::move(name)), declared_at(std::move(declared_at)), node(node)
    {
    }

    void Symbol::define(Core::Position *position) { defined_at = position; }

} // namespace Core
