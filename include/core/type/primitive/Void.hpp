#pragma once

#include "core/type/Type.hpp"

namespace Core
{
    class Void : public BaseType
    {
      private:
      public:
        Void(Environment *environment);

        bool
        assignable(const Core::Value &value,
                   const std::vector<TypeArgument> &arguments) const override;
        bool assignable_with(const BaseType &type) const override;
        std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
            AST::Node *node,
            const std::vector<TypeArgument> &arguments) const override;
    };

} // namespace Core
