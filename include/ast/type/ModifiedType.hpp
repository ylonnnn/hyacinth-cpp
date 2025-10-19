#pragma once

#include "ast/type/Type.hpp"

namespace AST
{
    enum class ModifierType
    {
        Mutable,
    };

    std::ostream &operator<<(std::ostream &os, ModifierType type);

    struct ModifiedType : Type
    {
        ModifierType type;
        std::unique_ptr<Type> base;

        ModifiedType(ModifierType type, std::unique_ptr<Type> &&base);

        std::string to_string() const override;
        void print(std::ostream &os, uint32_t tab) const override;
    };

} // namespace AST
