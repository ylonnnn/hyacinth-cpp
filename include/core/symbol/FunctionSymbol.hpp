#pragma once

#include "core/symbol/Symbol.hpp"
#include "core/type/Type.hpp"

namespace Core
{
    struct FunctionParameter
    {
        std::string_view name;
        bool is_mutable;
        Type *type;
    };

    struct FunctionSymbol : public Symbol
    {
        Type *return_type;
        std::vector<FunctionParameter> parameters;
    };

} // namespace Core
