#pragma once

#include "core/type/Type.hpp"
#include "core/value/Value.hpp"

namespace Core
{
    class BitWidthType : public BaseType
    {
      protected:
        uint min_;
        uint max_;

      public:
        BitWidthType();

      protected:
        bool
        assignable(const Core::Value &value,
                   const std::vector<TypeArgument> &arguments) const override;
        bool assignable_with(const BaseType &type) const override;
        std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
            AST::Node *node,
            const std::vector<TypeArgument> &arguments) const override;
    };

    class IntegerType : public BaseType
    {
      private:
        bool is_signed_;
        BitWidthType bw_type_;

      public:
        IntegerType(Environment *environment, bool is_signed);

        bool is_signed() const;

        bool
        assignable(const Core::Value &value,
                   const std::vector<TypeArgument> &arguments) const override;
        bool assignable_with(const BaseType &type) const override;
        std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
            AST::Node *node,
            const std::vector<TypeArgument> &arguments) const override;
    };

} // namespace Core
