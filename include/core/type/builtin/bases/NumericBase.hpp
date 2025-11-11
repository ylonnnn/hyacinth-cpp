#pragma once

#include "core/type/Type.hpp"

namespace Core
{
    struct NumericInstantiated : InstantiatedType
    {
        NumericInstantiated(BaseType &base,
                            std::vector<GenericArgument> &&arguments,
                            Core::PositionRange *range = nullptr);
        virtual ~NumericInstantiated() = default;

        virtual TypeResult assignable(Value *value) const override = 0;
        virtual TypeResult
        assignable(const InstantiatedType &type,
                   PositionRange *range = nullptr) const override;
    };

    struct NumericBase : BaseType
    {
        using T = NumericInstantiated;

        NumericBase(Core::Environment &environment, std::string &&name);
        virtual ~NumericBase() = default;

        virtual T *infer(Value &value) override = 0;

        virtual Signal assignable(const std::vector<GenericArgument> &arguments,
                                  Value *value,
                                  TypeResult &result) const override = 0;

        virtual T *create_instance(std::vector<GenericArgument> &&arguments,
                                   PositionRange *range = nullptr) override = 0;

        virtual std::unique_ptr<Diagnostic::Diagnostic>
        make_suggestion(const std::vector<GenericArgument> &arguments,
                        Value *value) const override = 0;

        // Type *construct_wrapper(
        //     std::vector<GenericArgument> &&arguments) const override = 0;
        // virtual Type *construct_wrapper(uint8_t bit_width) const = 0;

        // virtual Type *higher_bit_width(Type *a, Type *b) const;
    };

} // namespace Core
