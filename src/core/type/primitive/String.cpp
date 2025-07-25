#include <variant>

#include "core/type/Type.hpp"
#include "core/type/primitive/String.hpp"
#include "diagnostic/NoteDiagnostic.hpp"

namespace Core
{
    StringType::StringType(Environment *environment)
        : BaseType(environment, "str")
    {
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
            std::string("Only values that are ") + Diagnostic::NOTE_GEN +
                "Character Sequences" + Utils::Styles::Reset +
                " surrounded by " + Diagnostic::NOTE_GEN +
                "Double Quotation Marks" + Utils::Styles::Reset +
                " are accepted.",
            "Implement suggestion here");
    }

} // namespace Core
