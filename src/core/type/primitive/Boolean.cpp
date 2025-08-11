#include <functional>

#include "core/operation/Operation.hpp"
#include "core/type/Type.hpp"
#include "core/type/primitive/Boolean.hpp"

namespace Core
{
    BooleanType::BooleanType(Environment *environment)
        : BaseType(environment, "bool"), bool_w_(Type::get_or_create(this, {}))
    {
        builtin_ = true;

        default_operations();
    };

    void BooleanType::default_operations()
    {
        using namespace Lexer::TokenTypes;
        using namespace Operator;

        // Relational
        auto __nv = [&]([[maybe_unused]] Type *left,
                        [[maybe_unused]] Type *right) -> Type *
        { return Type::get_or_create(this, {}); };

        Operation::overload_binary(
            {Operation::BinarySignature{Relational::Equal, bool_w_, bool_w_},
             true,
             overload<bool, bool>([](bool a, bool b) -> bool { return a == b; },
                                  __nv)});

        Operation::overload_binary(
            {Operation::BinarySignature{Relational::NotEqual, bool_w_, bool_w_},
             true,
             overload<bool, bool>([](bool a, bool b) -> bool { return a != b; },
                                  __nv)});
    }

    bool BooleanType::assignable(
        const Core::Value &value,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        return std::holds_alternative<bool>(value);
    }

    bool BooleanType::assignable_with(const BaseType &type) const
    {
        return BaseType::assignable_with(type);
    }

    std::unique_ptr<Diagnostic::NoteDiagnostic> BooleanType::make_suggestion(
        AST::Node *node,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        return std::make_unique<Diagnostic::NoteDiagnostic>(
            node, Diagnostic::NoteType::Suggestion,
            std::string("Only values of type \"") + Diagnostic::NOTE_GEN +
                "boolean" + Utils::Styles::Reset + "\" are accepted.",
            "Use either \"true\" or \"false\".\n\t\t- Or a Locator Value "
            "(L-value) that holds a boolean.");
    }

    Type *BooleanType::from_value(const Core::Value &value) const
    {
        auto ptr = std::get_if<bool>(&value);
        if (ptr == nullptr)
            return nullptr;

        return Type::get_or_create(const_cast<BooleanType *>(this), {});
    }

} // namespace Core
