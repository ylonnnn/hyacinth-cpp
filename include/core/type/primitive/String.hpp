#pragma once

#include <functional>

#include "core/operation/Operation.hpp"
#include "core/type/Type.hpp"
#include "utils/value.hpp"

namespace Core
{
    class StringType : public BaseType
    {
      protected:
        Type *str_w_ = nullptr, *bool_w_ = nullptr, *int_w_ = nullptr;

      public:
        StringType(Environment *environment);

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
