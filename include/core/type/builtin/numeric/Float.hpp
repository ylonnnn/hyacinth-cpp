#pragma once

#include <array>

#include "core/type/Type.hpp"
#include "core/type/builtin/bases/NumericBase.hpp"
#include "core/type/builtin/numeric/BitWidth.hpp"

namespace Core
{
    struct FloatBitWidthType : BitWidthType
    {
        FloatBitWidthType(Environment &environment);

        T *create_instance(std::vector<GenericArgument> &&arguments,
                           Core::PositionRange *range = nullptr) override;

        Signal assignable(const std::vector<GenericArgument> &arguments,
                          Value *value, TypeResult &result) const override;

        T *infer(Value &value) override;

        std::unique_ptr<Diagnostic::Diagnostic>
        make_suggestion(const std::vector<GenericArgument> &arguments,
                        Value *value) const override;
    };

    struct FloatInstantiated : NumericInstantiated
    {
        FloatInstantiated(BaseType &base,
                          std::vector<GenericArgument> &&arguments,
                          Core::PositionRange *range = nullptr);

        TypeResult assignable(Value *value) const override;
        TypeResult assignable(const InstantiatedType &type,
                              PositionRange *range = nullptr) const override;
    };

    struct FloatType : NumericBase
    {
        using T = FloatInstantiated;
        static constexpr Signal Mismatch = 0, // Type Mismatch
            Underflow = 1,                    // Value Underflow
            Overflow = 2,                     // Value Overflow
            Assignable = 3;                   // Assignable

        FloatType(Environment &environment);
        ~FloatType();

        void default_operations() override;

        T *create_instance(std::vector<GenericArgument> &&arguments,
                           Core::PositionRange *range = nullptr) override;

        /**
         * Checks whether the value provided can fit with the given target
         * exponent and target mantissa
         *
         * @returns `0` for Assignable, `-1` for Underflow, `1` for Overflow,
         * and `-2` for invalid
         */
        int8_t can_fit(Value &value, uint64_t t_exponent,
                       uint64_t t_mantissa) const;

        Signal assignable(const std::vector<GenericArgument> &arguments,
                          Value *value, TypeResult &result) const override;

        T *infer(Value &value) override;

        std::unique_ptr<Diagnostic::Diagnostic>
        make_suggestion(const std::vector<GenericArgument> &arguments,
                        Value *value) const override;

        std::pair<uint32_t, std::pair<uint64_t, uint64_t>>
        bit_width_of(Value &value) const;

        // bool
        // assignable(const Core::Value &value,
        //            const std::vector<TypeArgument> &arguments) const
        //            override;
        // std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
        //     AST::Node *node,
        //     const std::vector<TypeArgument> &arguments) const override;

      private:
        FloatBitWidthType bw_type_;

        // [Exponent, Mantissa] Pairs
        std::array<std::pair<uint64_t, uint64_t>, 4> bit_pairs_ = {
            {{3, 4}, {5, 10}, {8, 23}, {11, 52}}};
    };

} // namespace Core
