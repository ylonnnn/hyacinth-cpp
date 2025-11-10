#pragma once

#include "core/type/Type.hpp"

namespace Core
{
    // *T
    // Arguments:
    //      * T - the pointer type

    struct PointerInstantiated : InstantiatedType
    {
        PointerInstantiated(BaseType &base,
                            std::vector<GenericArgument> &&arguments,
                            PositionRange *range = nullptr);

        TypeResult assignable(Value *value) const override;
    };

    struct PointerType : BaseType
    {
        using T = PointerInstantiated;

        static PointerType *instance(Environment *environment = nullptr);

        PointerType(Environment &environment);
        ~PointerType();

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
        static std::unique_ptr<PointerType> instance_;
    };

} // namespace Core
