#include <cassert>

#include "core/environment/Environment.hpp"
#include "core/environment/FunctionEnvironment.hpp"
#include "core/symbol/FunctionSymbol.hpp"

namespace Core
{
    FunctionEnvironment::FunctionEnvironment(Environment *parent,
                                             FunctionSymbol *fn)
        : Environment(parent, std::string(fn->name)), fn_(fn)
    {
        assert(fn != nullptr);
    }

    FunctionSymbol *FunctionEnvironment::fn_symbol() { return fn_; }

} // namespace Core
