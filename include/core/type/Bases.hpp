#pragma once

#include "core/type/Type.hpp"

namespace Core
{
    class NumericBase : public BaseType
    {
      public:
        NumericBase(Core::Environment *environment, std::string_view name);

      public:
        virtual void default_operations() = 0;

        Type *construct_wrapper(
            std::vector<TypeArgument> &&arguments) const override = 0;
        virtual Type *construct_wrapper(uint8_t bit_width) const = 0;

        virtual Type *higher_bit_width(Type *a, Type *b) const;
    };

} // namespace Core
