#pragma once

#include "core/type/Type.hpp"

namespace Core
{
    class StructType : public BaseType
    {
      private:
        std::unordered_map<std::string_view, Type *> fields_;

      public:
        StructType(Environment *environment, std::string_view name,
                   std::unordered_map<std::string_view, Type *> &&fields,
                   TypeSymbol *symbol = nullptr);

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

        std::unordered_map<std::string_view, Type *> &fields();
    };

} // namespace Core
