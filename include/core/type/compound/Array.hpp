#pragma once

#include "core/type/Type.hpp"

namespace Core
{
    class ArrayType : public BaseType
    {
      public:
        ArrayType(Environment *environment);

      protected:
        bool
        assignable(const Core::Value &value,
                   const std::vector<TypeArgument> &arguments) const override;

        std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
            AST::Node *node,
            const std::vector<TypeArgument> &arguments) const override;

      public:
        bool assignable_with(const BaseType &type) const override;

        Type *from_value(const Core::Value &value) const override;
    };

} // namespace Core
