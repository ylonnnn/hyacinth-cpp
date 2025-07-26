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
        std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
            AST::Node *node,
            const std::vector<TypeArgument> &arguments) const override;

      public:
        bool assignable_with(const BaseType &type) const override;
    };

    class IntegerType : public BaseType
    {
      private:
        class Wrapper : public Type
        {
          public:
            using Type::Type;

            bool assignable_with(const Type &type) const override;
        };

      private:
        bool is_signed_;
        BitWidthType bw_type_;

      public:
        IntegerType(Environment *environment, bool is_signed);

      protected:
        bool
        assignable(const Core::Value &value,
                   const std::vector<TypeArgument> &arguments) const override;
        std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
            AST::Node *node,
            const std::vector<TypeArgument> &arguments) const override;

      public:
        bool is_signed() const;

        std::unique_ptr<Type> construct_wrapper() const override;

        bool assignable_with(const BaseType &type) const override;
    };

} // namespace Core
