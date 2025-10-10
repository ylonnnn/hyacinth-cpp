#pragma once

#include "ast/type/Type.hpp"

namespace AST
{
    enum class PrefixedTypeKind
    {
        Array,
        Pointer,
        Reference,
        RValueReference,
    };

    std::ostream &operator<<(std::ostream &os, PrefixedTypeKind kind);

    struct PrefixedType : Type
    {
        PrefixedTypeKind kind;
        std::unique_ptr<Type> base;

        PrefixedType(PrefixedTypeKind kind, std::unique_ptr<Type> &&base);

        std::string to_string() const override;
        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
