#pragma once

#include "core/type/Type.hpp"

namespace Core
{
    struct NumericInstantiated : InstantiatedType
    {
        NumericInstantiated(BaseType &base,
                            std::vector<TypeArgument> &&arguments);
    };

    struct NumericBase : BaseType
    {
        using T = NumericInstantiated;

        NumericBase(Core::Environment &environment, std::string &&name);

        virtual void default_operations() override = 0;

        T *create_instance(std::vector<TypeArgument> &&arguments) override;

        virtual TypeResult
        assignable(const std::vector<TypeArgument> &arguments,
                   Value *value) const override = 0;

        // Type *construct_wrapper(
        //     std::vector<TypeArgument> &&arguments) const override = 0;
        // virtual Type *construct_wrapper(uint8_t bit_width) const = 0;

        // virtual Type *higher_bit_width(Type *a, Type *b) const;
    };

} // namespace Core
