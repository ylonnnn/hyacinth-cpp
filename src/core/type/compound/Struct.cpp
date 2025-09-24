#include "core/type/compound/Struct.hpp"
#include "core/environment/Environment.hpp"
#include "core/symbol/type/StructSymbol.hpp"
#include "core/type/Type.hpp"
#include "diagnostic/NoteDiagnostic.hpp"

namespace Core
{
    StructType::StructType(
        Environment *environment, std::string_view name,
        std::unordered_map<std::string_view, Type *> &&fields,
        TypeSymbol *symbol)
        : BaseType(environment, name, symbol), field_count_(fields.size())
    {
        members_.reserve(field_count_);

        for (const auto &[name, field] : fields)
            members_.try_emplace(std::string(name), field);
    }

    bool StructType::assignable(
        const Core::Value &value,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        auto instance_ptr = std::get_if<object>(&value);
        if (instance_ptr == nullptr)
            return false;

        object &instance = const_cast<object &>(*instance_ptr);
        if (instance.size() != members_.size())
            return false;

        for (const auto &[field, value] : instance.value())
        {
            auto it = members_.find(std::string(field));
            if (it == members_.end())
                return false;

            auto &member = const_cast<TypeMember &>(it->second);
            auto type = member.as<Type>();

            // Non-field member
            if (type == nullptr)
                return false;

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

        if (!BaseType::assignable_with(type))
            return false;

        auto &casted = static_cast<const StructType &>(type);
        if (field_count_ != casted.field_count_)
            return false;

        for (auto &[name, member] : members_)
        {
            auto type = const_cast<TypeMember &>(member).as<Type>();
            if (type == nullptr)
                return false;

            auto f_it = casted.members_.find(name);
            if (f_it == casted.members_.end())
                return false;

            auto f_type = const_cast<TypeMember &>(f_it->second).as<Type>();
            if (f_type == nullptr)
                return false;

            if (!f_type->assignable_with(*type))
                return false;
        }

        return true;
    }

    std::unique_ptr<Diagnostic::NoteDiagnostic> StructType::make_suggestion(
        AST::Node *node,
        [[maybe_unused]] const std::vector<TypeArgument> &arguments) const
    {
        std::string name(name_);

        auto diagnostic = std::make_unique<Diagnostic::NoteDiagnostic>(
            node, Diagnostic::NoteType::Suggestion,
            std::string("Only instances of \"") + Diagnostic::NOTE_GEN + name +
                utils::Styles::Reset + "\" are accepted.",
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
                    utils::Styles::Reset +
                    "\" is defined with the structure below.",
                "Use this definition as a guide"));
        }

        return diagnostic;
    }

    Type *StructType::from_value(const Core::Value &value) const
    {
        auto ptr = std::get_if<object>(&value);
        if (ptr == nullptr)
            return nullptr;

        // TODO: StructType::from_value

        return Type::get_or_create(const_cast<StructType *>(this), {});
    }

    size_t &StructType::field_count() { return field_count_; }

    size_t StructType::field_count() const { return field_count_; }

} // namespace Core
