#pragma once

#include "core/type/Bases.hpp"
#include "core/type/Type.hpp"
#include "core/type/primitive/Integer.hpp"

namespace Core
{
    class FloatBitWidthType : public BitWidthType
    {
      public:
        FloatBitWidthType();

      protected:
        bool
        assignable(const Core::Value &value,
                   const std::vector<TypeArgument> &arguments) const override;
        std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
            AST::Node *node,
            const std::vector<TypeArgument> &arguments) const override;

      public:
        bool assignable_with(const BaseType &type) const override;
    };

    class FloatType : public NumericBase
    {
      private:
        class Wrapper : public Type
        {
          public:
            using Type::Type;

            bool assignable_with(const Type &type) const override;
        };

      private:
        bool is_signed_;
        FloatBitWidthType bw_type_;

        Wrapper *float_w_ = nullptr;
        Type *bool_w_ = nullptr;

      public:
        FloatType(Environment *environment);

      protected:
        void default_operations() override;

        bool can_fit(double value, uint bits, uint mantissa) const;

        bool
        assignable(const Core::Value &value,
                   const std::vector<TypeArgument> &arguments) const override;
        std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
            AST::Node *node,
            const std::vector<TypeArgument> &arguments) const override;

      public:
        Type *construct_wrapper() const override;
        Type *construct_wrapper(uint8_t bit_width) const override;

        Type *from_value(const Core::Value &value) const override;

        bool assignable_with(const BaseType &type) const override;
    };

} // namespace Core
