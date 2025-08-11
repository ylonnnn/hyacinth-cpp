#pragma once

#include "core/type/Type.hpp"

namespace Core
{
    class Void : public BaseType
    {
      public:
        Void(Environment *environment);

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
        }

        bool assignable_with(const BaseType &type) const override;
    };

} // namespace Core
