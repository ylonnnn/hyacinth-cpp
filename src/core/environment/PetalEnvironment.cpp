#include <cassert>

#include "core/environment/Environment.hpp"
#include "core/environment/PetalEnvironment.hpp"

namespace Core
{
    PetalEnvironment::PetalEnvironment(Environment *parent, PetalSymbol *petal)
        : Environment(parent), petal(petal)
    {
    }

    // Symbol *LibEnvironment::resolve_symbol(const std::string &name,
    //                                        size_t depth)
    // {
    //     Symbol *resolved = Environment::resolve_symbol(name, depth - 1);
    //     if (resolved == nullptr ||
    //         resolved->accessibility == SymbolAccessibility::Private)
    //         return nullptr;

    //     return resolved;
    // }

    // BaseType *LibEnvironment::resolve_type(const std::string &name,
    //                                        size_t depth)
    // {
    //     BaseType *resolved = Environment::resolve_type(name, depth - 1);
    //     if (resolved == nullptr ||
    //         (resolved->symbol() &&
    //          resolved->symbol()->accessibility ==
    //          SymbolAccessibility::Private))
    //         return parent_ == nullptr ? nullptr
    //                                   : parent_->resolve_type(name, depth -
    //                                   1);

    //     return resolved;
    // }

    // VariableSymbol *LibEnvironment::resolve_variable(const std::string &name,
    //                                                  size_t depth)
    // {
    //     VariableSymbol *resolved =
    //         Environment::resolve_variable(name, depth - 1);
    //     if (resolved == nullptr ||
    //         resolved->accessibility == SymbolAccessibility::Private)
    //         return nullptr;

    //     return resolved;
    // }

} // namespace Core
