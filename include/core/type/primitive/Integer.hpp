#pragma once

#include "core/type/Bases.hpp"
#include "core/type/Type.hpp"
#include "core/value/Value.hpp"

namespace Core
{
    class BitWidthType : public BaseType
    {
      protected:
        uint64_t min_;
        uint64_t max_;

      public:
        BitWidthType();

      protected:
        bool
        assignable(const Core::Value &value,
                   const std::vector<TypeArgument> &arguments) const override;
        std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
            AST::Node *node,
            const std::vector<TypeArgument> &arguments) const override;

      public:
        Type *
        from_value([[maybe_unused]] const Core::Value &value) const override
        {
            return nullptr;
        };

        bool assignable_with(const BaseType &type) const override;
    };

    class IntegerType : public NumericBase
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
        BitWidthType bw_type_;

        Wrapper *int_w_ = nullptr;
        Type *bool_w_ = nullptr;

      public:
        IntegerType(Environment *environment, bool is_signed);
        ~IntegerType();

      protected:
        void default_operations() override;

        bool
        assignable(const Core::Value &value,
                   const std::vector<TypeArgument> &arguments) const override;
        std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
            AST::Node *node,
            const std::vector<TypeArgument> &arguments) const override;

      public:
        bool is_signed() const;

        Type *
        construct_wrapper(std::vector<TypeArgument> &&arguments) const override;
        Type *construct_wrapper(uint8_t bit_width) const override;

        Type *from_value(const Core::Value &value) const override;

        bool assignable_with(const BaseType &type) const override;
    };

} // namespace Core
