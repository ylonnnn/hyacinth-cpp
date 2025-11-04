#pragma once

#include "core/type/Type.hpp"

namespace Core
{
    // []T | [N]T
    // Arguments:
    //      * N - array element count
    //      * T - array element type

    struct ArrayInstantiated : InstantiatedType
    {
        ArrayInstantiated(BaseType &base,
                          std::vector<GenericArgument> &&arguments,
                          Core::PositionRange *range = nullptr);

        TypeResult assignable(Value *value) const override;
    };

    struct ArrayType : BaseType
    {
        using T = ArrayInstantiated;

        static ArrayType *instance(Environment *environment = nullptr);

        ArrayType(Environment &environment);
        ~ArrayType();

        void default_operations() override;

        T *create_instance(std::vector<GenericArgument> &&arguments,
                           Core::PositionRange *range = nullptr) override;

        Signal assignable(const std::vector<GenericArgument> &arguments,
                          Value *value, TypeResult &result) const override;

        T *infer(Value &value) override;

        std::unique_ptr<Diagnostic::Diagnostic>
        make_suggestion(const std::vector<GenericArgument> &arguments,
                        Value *value) const override;

      private:
        static std::unique_ptr<ArrayType> instance_;
    };

} // namespace Core
