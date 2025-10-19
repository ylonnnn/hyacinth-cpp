#pragma once

#include "ast/type/Type.hpp"

namespace AST
{
    enum class SuffixedTypeKind
    {
        Optional,
    };

    struct SuffixedType : Type
    {
        SuffixedTypeKind kind;
        std::unique_ptr<Type> base;

        SuffixedType(SuffixedTypeKind kind, std::unique_ptr<Type> &&base);

        std::string to_string() const override;
        void print(std::ostream &os, uint32_t tab) const override;
    };

} // namespace AST
