#include <cassert>

#include "core/environment/FunctionEnvironment.hpp"
#include "core/environment/Environment.hpp"
#include "core/symbol/FunctionSymbol.hpp"

namespace Core
{
    FunctionEnvironment::FunctionEnvironment(Environment *parent,
                                             FunctionSymbol *fn)
        : Environment(parent), fn_(fn)
    {
        assert(fn != nullptr);
    }

    FunctionSymbol *FunctionEnvironment::fn_symbol() { return fn_; }

} // namespace Core
