#pragma once

#include "core/type/Type.hpp"

namespace Core
{
    class CharacterType : public BaseType
    {
      public:
        CharacterType(Environment *environment);

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

} // namespace Core
