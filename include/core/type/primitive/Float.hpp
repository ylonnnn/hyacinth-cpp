#pragma once

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

    class FloatType : public BaseType
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

      public:
        FloatType(Environment *environment);

      protected:
        bool can_fit(double value, uint bits, uint mantissa) const;

        bool
        assignable(const Core::Value &value,
                   const std::vector<TypeArgument> &arguments) const override;
        std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
            AST::Node *node,
            const std::vector<TypeArgument> &arguments) const override;

      public:
        std::unique_ptr<Type> construct_wrapper() const override;

        bool assignable_with(const BaseType &type) const override;
    };

} // namespace Core
