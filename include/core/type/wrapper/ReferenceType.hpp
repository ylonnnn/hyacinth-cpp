#pragma once

#include "core/type/Type.hpp"

namespace Core
{
    // &T
    // Arguments:
    //      * T - the reference type

    struct ReferenceInstantiated : InstantiatedType
    {
        ReferenceInstantiated(BaseType &base,
                              std::vector<GenericArgument> &&arguments,
                              PositionRange *range = nullptr);

        TypeResult assignable(Value *value) const override;
    };

    struct ReferenceType : BaseType
    {
        using T = ReferenceInstantiated;

        static ReferenceType *instance(Environment *environment = nullptr);

        ReferenceType(Environment &environment);
        ~ReferenceType();

        void default_operations() override;

        T *create_instance(std::vector<GenericArgument> &&arguments,
                           PositionRange *range = nullptr) override;

        Signal assignable(const std::vector<GenericArgument> &arguments,
                          Value *value, TypeResult &result) const override;

        T *infer(Value &value) override;

        std::unique_ptr<Diagnostic::Diagnostic>
        make_suggestion(const std::vector<GenericArgument> &arguments,
                        Value *value) const override;

      private:
        static std::unique_ptr<ReferenceType> instance_;
    };

} // namespace Core
