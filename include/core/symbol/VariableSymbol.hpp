#pragma once

#include "core/symbol/Symbol.hpp"
#include "core/type/Type.hpp"
#include "core/value/Value.hpp"

namespace Core
{
    struct VariableSymbol : public Symbol
    {
        bool is_mutable;
        Type *type;
        Value value;
    };

} // namespace Core
