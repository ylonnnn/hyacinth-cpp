#include "core/type/primitive/Void.hpp"
#include "core/type/Type.hpp"

namespace Core
{
    Void::Void(Environment *environment) : BaseType(environment, "void")
    {
        builtin_ = true;
    }

    bool Void::assignable(
        const Core::Value &value,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        return std::holds_alternative<Core::null>(value);
    }

    bool Void::assignable_with([[maybe_unused]] const BaseType &type) const
    {
        return false;
    }

    std::unique_ptr<Diagnostic::NoteDiagnostic> Void::make_suggestion(
        AST::Node *node,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        return std::make_unique<Diagnostic::NoteDiagnostic>(
            node, Diagnostic::NoteType::Suggestion,
            "Must not return any value.", "Returns a value here");
    }

} // namespace Core
