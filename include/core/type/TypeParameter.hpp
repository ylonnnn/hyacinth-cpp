#pragma once

#include <string>

namespace Core
{
    struct BaseType;
    struct InstantiatedType;

    enum class TypeParameterType
    {
        Type,
        Value,
    };

    struct TypeParameter
    {
        TypeParameterType param_type;
        std::string name;
        InstantiatedType *type;
    };

} // namespace Core
