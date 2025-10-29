#pragma once

#include "core/type/Type.hpp"
#include "core/type/builtin/bases/NumericBase.hpp"
#include "core/type/builtin/numeric/BitWidth.hpp"
#include "core/value/Value.hpp"

namespace Core
{
    struct IntegerType : NumericBase
    {
        bool is_signed;

        //   Wrapper *int_w_ = nullptr;
        //   Type *bool_w_ = nullptr;

        IntegerType(Environment &environment, bool is_signed);
        ~IntegerType();

        void default_operations() override;

        TypeResult assignable(const std::vector<TypeArgument> &arguments,
                              Value *value) const override;

      private:
        BitWidthType bw_type_;

        // bool
        // assignable(const Core::Value &value,
        //            const std::vector<TypeArgument> &arguments) const
        //            override;

        // std::unique_ptr<Diagnostic::NoteDiagnostic> make_suggestion(
        //     AST::Node *node,
        //     const std::vector<TypeArgument> &arguments) const override;

        // bool is_signed() const;

        // Type *
        // construct_wrapper(std::vector<TypeArgument> &&arguments) const
        // override; Type *construct_wrapper(uint8_t bit_width) const override;

        // Type *from_value(const Core::Value &value) const override;

        // bool assignable_with(const BaseType &type) const override;
    };

} // namespace Core
