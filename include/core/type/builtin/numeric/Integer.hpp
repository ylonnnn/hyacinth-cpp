#pragma once

#include "core/type/Type.hpp"
#include "core/type/builtin/bases/NumericBase.hpp"
#include "core/type/builtin/numeric/BitWidth.hpp"
#include "core/value/Value.hpp"

namespace Core
{
    struct IntegerInstantiated : NumericInstantiated
    {
        IntegerInstantiated(BaseType &base,
                            std::vector<GenericArgument> &&arguments);

        TypeResult assignable(Value *value) const override;
    };

    struct IntegerType : NumericBase
    {
        using T = IntegerInstantiated;
        static constexpr Signal Mismatch = 0, // Type Mismatch
            Underflow = 1,                    // Value Underflow
            Overflow = 2,                     // Value Overflow
            Assignable = 3;                   // Assignable

        bool is_signed;

        //   Wrapper *int_w_ = nullptr;
        //   Type *bool_w_ = nullptr;

        IntegerType(Environment &environment, bool is_signed);
        ~IntegerType();

        void default_operations() override;

        T *create_instance(std::vector<GenericArgument> &&arguments) override;

        Signal assignable(const std::vector<GenericArgument> &arguments,
                          Value *value, TypeResult &result) const override;

        std::unique_ptr<Diagnostic::Diagnostic>
        make_suggestion(const std::vector<GenericArgument> &arguments,
                        Value *value) const override;

      private:
        BitWidthType bw_type_;

        // bool
        // assignable(const Core::Value &value,
        //            const std::vector<GenericArgument> &arguments) const
        //            override;

        // std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
        //     AST::Node *node,
        //     const std::vector<GenericArgument> &arguments) const override;

        // bool is_signed() const;

        // Type *
        // construct_wrapper(std::vector<GenericArgument> &&arguments) const
        // override; Type *construct_wrapper(uint8_t bit_width) const override;

        // Type *from_value(const Core::Value &value) const override;

        // bool assignable_with(const BaseType &type) const override;
    };

} // namespace Core
