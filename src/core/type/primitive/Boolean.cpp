#include "core/type/primitive/Boolean.hpp"
#include "core/type/Type.hpp"

namespace Core
{
    BooleanType::BooleanType(Environment *environment)
        : BaseType(environment, "bool") {};

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

} // namespace Core
