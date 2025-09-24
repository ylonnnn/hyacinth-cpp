#include <functional>

#include "core/environment/Environment.hpp"
#include "core/operation/Operation.hpp"
#include "core/type/primitive/Character.hpp"

namespace Core
{
    CharacterType::CharacterType(Environment *environment)
        : BaseType(environment, "char"), char_w_(Type::get_or_create(this, {})),
          bool_w_(Type::get_or_create(environment_->resolve_type("bool"), {}))
    {
        builtin_ = true;

        default_operations();
    }

    void CharacterType::default_operations()
    {
        using namespace Lexer::TokenTypes;
        using namespace Operator;

        // Relational
        auto __nv = [&]([[maybe_unused]] Type *left,
                        [[maybe_unused]] Type *right) -> Type *
        { return bool_w_; };

        Operation::overload_binary(
            {Operation::BinarySignature{Relational::Equal, char_w_, char_w_},
             true,
             overload<char, char>([](char a, char b) -> bool { return a == b; },
                                  __nv)});

        Operation::overload_binary(
            {Operation::BinarySignature{Relational::NotEqual, char_w_, char_w_},
             true,
             overload<char, char>([](char a, char b) -> bool { return a != b; },
                                  __nv)});
    }

    bool CharacterType::assignable(
        const Core::Value &value,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        return std::holds_alternative<char>(value);
    }

    bool CharacterType::assignable_with(const BaseType &type) const
    {
        return BaseType::assignable_with(type);
    }

    std::unique_ptr<Diagnostic::NoteDiagnostic> CharacterType::make_suggestion(
        AST::Node *node,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        return std::make_unique<Diagnostic::NoteDiagnostic>(
            node, Diagnostic::NoteType::Suggestion,
            std::string("Only values of type \"") + Diagnostic::NOTE_GEN +
                "character" + utils::Styles::Reset + "\" are accepted.",
            "Use a single character surrounded by single quotation "
            "marks.\n\t\t- Or a Locator Value (L-value) that holds a "
            "character.");
    }

    Type *CharacterType::from_value(const Core::Value &value) const
    {
        auto ptr = std::get_if<char>(&value);
        if (ptr == nullptr)
            return nullptr;

        return Type::get_or_create(const_cast<CharacterType *>(this), {});
    }

} // namespace Core
