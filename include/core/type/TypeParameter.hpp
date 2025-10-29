#pragma once

#include <string>

namespace Core
{
    struct InstantiatedType;
    struct BaseType;

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

        TypeParameter(TypeParameterType param_type, std::string &&name,
                      InstantiatedType *type)
            : param_type(param_type), name(std::move(name)), type(type)
        {
        }
    };

} // namespace Core
