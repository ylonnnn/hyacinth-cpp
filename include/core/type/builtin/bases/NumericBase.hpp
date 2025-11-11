#pragma once

#include "core/type/Type.hpp"

namespace Core
{
    struct NumericInstantiated : InstantiatedType
    {
        NumericInstantiated(BaseType &base,
                            std::vector<GenericArgument> &&arguments,
                            Core::PositionRange *range = nullptr);

        virtual TypeResult assignable(Value *value) const override = 0;
        virtual TypeResult
        assignable(const InstantiatedType &type,
                   PositionRange *range = nullptr) const override = 0;
    };

    struct NumericBase : BaseType
    {
        using T = NumericInstantiated;

        NumericBase(Core::Environment &environment, std::string &&name);
        virtual ~NumericBase() = default;

        // Type *construct_wrapper(
        //     std::vector<GenericArgument> &&arguments) const override = 0;
        // virtual Type *construct_wrapper(uint8_t bit_width) const = 0;

        // virtual Type *higher_bit_width(Type *a, Type *b) const;
    };

} // namespace Core
