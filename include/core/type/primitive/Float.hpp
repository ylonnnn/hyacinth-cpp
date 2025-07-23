#pragma once

#include "core/type/Type.hpp"
#include "core/type/primitive/Integer.hpp"

namespace Core
{
    class FloatType : public Type
    {
      private:
        bool is_signed_;
        BitWidthType bw_type_;

      public:
        FloatType(Environment *environment);

        bool
        assignable(const Core::Value &value,
                   const std::vector<TypeArgument> &arguments) const override;
        bool assignable_with(const Type &type) const override;
        std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
            AST::Node *node,
            const std::vector<TypeArgument> &arguments) const override;
    };

} // namespace Core
