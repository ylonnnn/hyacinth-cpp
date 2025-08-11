#pragma once

#include "core/type/Type.hpp"

namespace Core
{
    class CharacterType : public BaseType
    {
      private:
        Type *char_w_ = nullptr, *bool_w_ = nullptr;

      public:
        CharacterType(Environment *environment);

      protected:
        void default_operations();

        bool
        assignable(const Core::Value &value,
                   const std::vector<TypeArgument> &arguments) const override;
        std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
            AST::Node *node,
            const std::vector<TypeArgument> &arguments) const override;

      public:
        Type *from_value(const Core::Value &value) const override;

        bool assignable_with(const BaseType &type) const override;
    };

} // namespace Core
