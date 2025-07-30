#include "core/type/compound/Struct.hpp"
#include "core/environment/Environment.hpp"
#include "core/symbol/type/StructSymbol.hpp"
#include "core/type/Type.hpp"
#include "diagnostic/NoteDiagnostic.hpp"

namespace Core
{
    StructType::StructType(
        Environment *environment, std::string_view name,
        std::unordered_map<std::string_view, std::unique_ptr<Type>> &&fields)
        : BaseType(environment, name)
    {
        fields_.reserve(fields.size());

        for (auto &[name, field] : fields)
            fields_.try_emplace(name, std::move(field));
    }

    bool StructType::assignable(
        const Core::Value &value,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        auto instance_ptr = std::get_if<object>(&value);
        if (instance_ptr == nullptr)
            return false;

        object &instance = const_cast<object &>(*instance_ptr);
        if (instance.size() != fields_.size())
            return false;

        for (const auto &[field, value] : instance.value())
        {
            auto it = fields_.find(field);
            if (it == fields_.end())
                return false;

            auto &type = it->second;
            if ((value.value && type->assignable(*value.value)) ||
                (value.type && type->assignable_with(*value.type)))
                continue;

            return false;
        }

        return true;
    }

    bool StructType::assignable_with(const BaseType &type) const
    {
        // TODO: Allow types that inherit this type

        return BaseType::assignable_with(type);
    }

    std::unique_ptr<Diagnostic::NoteDiagnostic> StructType::make_suggestion(
        AST::Node *node,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        std::string name(name_);

        auto diagnostic = std::make_unique<Diagnostic::NoteDiagnostic>(
            node, Diagnostic::NoteType::Suggestion,
            std::string("Only instances of \"") + Diagnostic::NOTE_GEN + name +
                Utils::Styles::Reset + "\" are accepted.",
            "Use an instance of struct \"" + name +
                "\" or an object with the same structure.");

        Core::Symbol *symbol = environment_->resolve_symbol(name);
        if (symbol != nullptr)
        {
            Core::StructSymbol *_struct =
                static_cast<Core::StructSymbol *>(symbol);

            diagnostic->add_detail(std::make_unique<Diagnostic::NoteDiagnostic>(
                _struct->defined_at != nullptr ? _struct->definition
                                               : _struct->node,
                Diagnostic::NoteType::Definition,
                "Struct \"" + (Diagnostic::NOTE_GEN + name) +
                    Utils::Styles::Reset +
                    "\" is defined with the structure below.",
                "Use this definition as a guide"));
        }

        return diagnostic;
    }

    std::unordered_map<std::string_view, std::unique_ptr<Type>> &
    StructType::fields()
    {
        return fields_;
    }

} // namespace Core
