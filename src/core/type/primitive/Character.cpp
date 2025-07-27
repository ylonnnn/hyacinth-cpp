#include "core/type/primitive/Character.hpp"

namespace Core
{
    CharacterType::CharacterType(Environment *environment)
        : BaseType(environment, "char")
    {
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
            std::string("Only values of type \"") + Diagnostic::ERR_GEN +
                "character" + Utils::Styles::Reset + "\" are accepted.",
            "Use a single character surrounded by single quotation "
            "marks.\n\t\t- Or a Locator Value (L-value) that holds a "
            "character.");
    }

} // namespace Core
