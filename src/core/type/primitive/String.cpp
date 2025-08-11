#include <functional>
#include <variant>

#include "core/environment/Environment.hpp"
#include "core/operation/Operation.hpp"
#include "core/type/Type.hpp"
#include "core/type/primitive/String.hpp"
#include "diagnostic/NoteDiagnostic.hpp"

namespace Core
{
    StringType::StringType(Environment *environment)
        : BaseType(environment, "str"), str_w_(Type::get_or_create(this, {})),
          bool_w_(Type::get_or_create(environment_->resolve_type("bool"), {})),
          int_w_(Type::get_or_create(environment_->resolve_type("int"), {}))
    {
        builtin_ = true;

        default_operations();
    }

    void StringType::default_operations()
    {
        using namespace Lexer::TokenTypes;
        using namespace Operator;

        auto __nv_ar = [&](Type *, Type *) -> Type * { return str_w_; };

        Operation::overload_binary(
            {Operation::BinarySignature{ArithmeticUnary::Plus, str_w_, str_w_},
             true,
             overload<std::string, std::string>(
                 [](const std::string &a, const std::string &b) -> std::string
                 { return a + b; }, __nv_ar)});

        Operation::overload_binary(
            {Operation::BinarySignature{Arithmetic::Multiplication, str_w_,
                                        int_w_},
             true,
             overload<std::string, h_int>(
                 [](const std::string &a, const h_int &b) -> std::string
                 {
                     std::string str;
                     if (b.is_signed() && b.i64() < 1)
                         return str;

                     for (size_t i = 0; i < b.u64(); i++)
                         str += a;

                     return str;
                 },
                 __nv_ar)});

        // Relational
        auto __nv_rel = [&](Type *, Type *) -> Type * { return bool_w_; };

        Operation::overload_binary(
            {Operation::BinarySignature{Relational::Equal, str_w_, str_w_},
             true,
             overload<std::string, std::string>(
                 [](const std::string &a, const std::string &b) -> bool
                 { return a == b; }, __nv_rel)});

        Operation::overload_binary(
            {Operation::BinarySignature{Relational::NotEqual, str_w_, str_w_},
             true,
             overload<std::string, std::string>(
                 [](const std::string &a, const std::string &b) -> bool
                 { return a != b; }, __nv_rel)});
    }

    bool StringType::assignable(
        const Core::Value &value,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        return std::holds_alternative<std::string>(value);
    }

    bool StringType::assignable_with(const BaseType &type) const
    {
        return BaseType::assignable_with(type);
    }

    std::unique_ptr<Diagnostic::NoteDiagnostic> StringType::make_suggestion(
        AST::Node *node,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        return std::make_unique<Diagnostic::NoteDiagnostic>(
            node, Diagnostic::NoteType::Suggestion,
            std::string("Only values of type \"") + Diagnostic::NOTE_GEN +
                "string" + Utils::Styles::Reset + "\" are accepted.",
            "Use sequences of characters surrounded by double quotation "
            "marks.\n\t\t- Or a Locator Value (L-value) that holds a string.");
    }

    Type *StringType::from_value(const Core::Value &value) const
    {
        auto ptr = std::get_if<std::string>(&value);
        if (ptr == nullptr)
            return nullptr;

        return Type::get_or_create(const_cast<StringType *>(this), {});
    }

} // namespace Core
