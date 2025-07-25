#pragma once

#include "core/type/Type.hpp"
#include "core/type/primitive/Integer.hpp"

namespace Core
{
    class FloatBitWidthType : public BitWidthType
    {
      public:
        FloatBitWidthType();

        bool
        assignable(const Core::Value &value,
                   const std::vector<TypeArgument> &arguments) const override;
        bool assignable_with(const BaseType &type) const override;
        std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
            AST::Node *node,
            const std::vector<TypeArgument> &arguments) const override;
    };

    class FloatType : public BaseType
    {
      private:
        bool is_signed_;
        FloatBitWidthType bw_type_;

      public:
        FloatType(Environment *environment);

        bool can_fit(double value, uint bits, uint mantissa) const;

        bool
        assignable(const Core::Value &value,
                   const std::vector<TypeArgument> &arguments) const override;
        bool assignable_with(const BaseType &type) const override;
        std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
            AST::Node *node,
            const std::vector<TypeArgument> &arguments) const override;
    };

} // namespace Core
