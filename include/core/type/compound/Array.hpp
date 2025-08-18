#pragma once

#include "core/type/Type.hpp"

namespace Core
{
    class ArrayType : public BaseType
    {
      public:
        class Wrapper : public Type
        {
          private:
            Type *element_type_ = nullptr;

          public:
            Wrapper(BaseType *base, std::vector<TypeArgument> &&arguments);

            Type *element_type();
            const Type *element_type() const;
        };

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

        Type *
        construct_wrapper(std::vector<TypeArgument> &&arguments) const override;

        Type *from_value(const Core::Value &value) const override;
    };

} // namespace Core
