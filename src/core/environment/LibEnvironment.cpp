#include <cassert>

#include "core/environment/Environment.hpp"
#include "core/environment/LibEnvironment.hpp"
#include "core/symbol/LibSymbol.hpp"

namespace Core
{
    LibEnvironment::LibEnvironment(Environment *parent, LibSymbol *lib)
        : Environment(parent, std::string(lib->name)), lib_(lib)
    {
        assert(lib != nullptr);
    }

    LibSymbol *LibEnvironment::lib_symbol() { return lib_; }

    const LibSymbol *LibEnvironment::lib_symbol() const { return lib_; }

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
