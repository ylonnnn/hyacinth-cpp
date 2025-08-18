#include "core/type/compound/Array.hpp"
#include "core/type/Type.hpp"

namespace Core
{
    ArrayType::ArrayType(Environment *environment)
        : BaseType(environment, "array")
    {
        builtin_ = true;

        add_parameter(TypeParameter{"_Tp", TypeParameterType::Type});
    }

    bool ArrayType::assignable(const Core::Value &value,
                               const std::vector<TypeArgument> &arguments) const
    {
        if (arguments.empty())
            return false;

        const TypeArgument &arg = arguments[0];
        auto a_ptr = std::get_if<Type *>(&arg);

        if (a_ptr == nullptr)
            return false;

        const Type *type = *a_ptr;

        auto ptr = std::get_if<Core::array>(&value);
        if (ptr == nullptr)
            return false;

        const Core::array &val = *ptr;
        const Type *el_type = val.element_type();

        if (el_type == nullptr)
            return val.elements().empty();

        if (!type->assignable_with(*el_type))
            return false;

        return true;
    }

    std::unique_ptr<Diagnostic::NoteDiagnostic> ArrayType::make_suggestion(
        AST::Node *node,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        return nullptr;
    }

    bool ArrayType::assignable_with(const BaseType &other) const
    {
        return BaseType::assignable_with(other);
    }

    Type *ArrayType::from_value(const Core::Value &value) const
    {
        auto ptr = std::get_if<array>(&value);
        if (ptr == nullptr)
            return nullptr;

        const array &val = *ptr;
        return Type::get_or_create(const_cast<ArrayType *>(this),
                                   {const_cast<Type *>(val.element_type())});
    }

} // namespace Core
