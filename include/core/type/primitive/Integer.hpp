#pragma once

#include "core/type/Type.hpp"
#include "core/value/Value.hpp"

namespace Core
{
    class BitWidthType : public Type
    {
      public:
        BitWidthType();

        bool
        assignable(const Core::Value &value,
                   const std::vector<TypeArgument> &arguments) const override;
        bool assignable_with(const Type &type) const override;
        std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
            AST::Node *node,
            const std::vector<TypeArgument> &arguments) const override;
    };

    class IntegerType : public Type
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
        bool assignable_with(const Type &type) const override;
        std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
            AST::Node *node,
            const std::vector<TypeArgument> &arguments) const override;
    };

} // namespace Core
