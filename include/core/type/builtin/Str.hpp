#pragma once

#include "core/type/Type.hpp"

namespace Core
{
    struct StrType : BaseType
    {
        StrType(Environment &environment);

        void default_operations() override;

        T *create_instance(std::vector<GenericArgument> &&arguments,
                           Core::PositionRange *range = nullptr) override;

        Signal assignable(const std::vector<GenericArgument> &arguments,
                          Value *value, TypeResult &result) const override;

        T *infer(Value &value) override;

        std::unique_ptr<Diagnostic::Diagnostic>
        make_suggestion(const std::vector<GenericArgument> &arguments,
                        Value *value) const override;
    };

} // namespace Core
