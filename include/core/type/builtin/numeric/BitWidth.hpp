#pragma once

#include "core/type/Type.hpp"

namespace Core
{
    struct BitWidthType : BaseType
    {
        static constexpr Signal Mismatch = 0, // Type Mismatch
            Assignable = 1;                   // Assignable

        BitWidthType(Environment &environment);

        T *create_instance(std::vector<GenericArgument> &&arguments) override;

        Signal assignable(const std::vector<GenericArgument> &arguments,
                          Value *value, TypeResult &result) const override;

        std::unique_ptr<Diagnostic::Diagnostic>
        make_suggestion(const std::vector<GenericArgument> &arguments,
                        Value *value) const override;

        // std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
        //     AST::Node *node,
        //     const std::vector<GenericArgument> &arguments) const override;

        // Type *
        // from_value([[maybe_unused]] const Core::Value &value) const override
        // {
        //     return nullptr;
        // };

      protected:
        uint64_t min_;
        uint64_t max_;
    };

} // namespace Core
