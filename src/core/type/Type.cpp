#include <iostream>
#include <string>

#include "ast/type/GenericType.hpp"
#include "core/environment/Environment.hpp"
#include "core/result/Result.hpp"
#include "core/type/Type.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "utils/value.hpp"

namespace Core
{
    Type::Type(BaseType *type, std::vector<TypeArgument> arguments)
        : type(type), arguments(std::move(arguments))
    {
    }

    bool Type::assignable(const Core::Value &value) const
    {
        return type->assignable(value, arguments);
    }

    bool Type::assignable_with(const Type &type) const
    {
        if (type.type == nullptr)
            return false;

        return this->type->assignable_with(*type.type);
    }

    std::unique_ptr<Diagnostic::NoteDiagnostic>
    Type::make_suggestion(AST::Node *node) const
    {
        return type->make_suggestion(node, arguments);
    }

    std::string Type::to_string() const
    {
        std::string str(type->name_);

        if (arguments.empty())
            return str;

        str += "[";

        for (auto &argument : arguments)
        {
            if (auto ptr = std::get_if<Core::Value>(&argument))
            {
                std::visit(
                    [&str](const auto &val) -> void
                    {
                        using T = std::decay_t<decltype(val)>;

                        if constexpr (std::is_convertible_v<T, std::string>)
                            str += std::string(val);
                        else
                            str += std::to_string(val);
                    },
                    *ptr);
            }

            else if (auto ptr = std::get_if<Type>(&argument))
                str += ptr->to_string();

            if (&argument != &arguments.back())
                str += ", ";
        }

        return str += "]";
    }

    TypeParameter::TypeParameter(std::string_view name,
                                 TypeParameterType param_type,
                                 std::optional<Type> type)
        : name(name), param_type(type ? param_type : TypeParameterType::Type),
          type(std::move(type))
    {
    }

    TypeResolutionResult::TypeResolutionResult(
        ResultStatus status, std::unique_ptr<Type> data,
        Diagnostic::DiagnosticList diagnostics)
        : Core::Result<std::unique_ptr<Type>>(status, std::move(data),
                                              std::move(diagnostics))
    {
    }

    TypeArgument TypeParameter::resolve(Environment &environment,
                                        AST::Type &type)
    {
        switch (param_type)
        {
            case Core::TypeParameterType::Type:
            {
                BaseType *resolved =
                    environment.resolve_type(std::string(type.value().value));
                if (resolved == nullptr || this->type == std::nullopt)
                    return Type(resolved, {});

                return Type(this->type->type->assignable_with(*resolved)
                                ? resolved
                                : nullptr,
                            {});
            }

            case Core::TypeParameterType::Constant:
            {
                Core::Value parsed = Utils::parse_val(type.value());
                if (this->type == std::nullopt)
                    return Core::null{};

                return this->type->assignable(parsed) ? parsed : Core::null{};
            }
        }
    }

    BaseType::BaseType(Environment *environment, std::string_view name)
        : environment_(environment), name_(name)
    {
        parameters_.reserve(8);
    }

    std::string_view BaseType::name() const { return name_; }

    std::vector<TypeParameter> &BaseType::parameters() { return parameters_; }

    void BaseType::create_parameter(std::string_view name,
                                    TypeParameterType param_type,
                                    std::optional<Type> type)
    {
        parameters_.push_back(TypeParameter{name, param_type, std::move(type)});
    }

    std::pair<bool, TypeArgument> BaseType::resolve_argument(size_t param_idx,
                                                             AST::Type &type)
    {
        std::pair<bool, TypeArgument> result = {false, Type(nullptr, {})};

        if (param_idx >= parameters_.size())
            return result;

        TypeParameter &param = parameters_[param_idx];
        TypeArgument resolved = param.resolve(*environment_, type);

        if (auto ptr = std::get_if<Type>(&resolved))
        {
            if (ptr->type == nullptr)
                return result;

            result.first = true;
            result.second = std::move(*ptr);

            return result;
        }

        else if (auto ptr = std::get_if<Core::Value>(&resolved))
        {
            if (std::holds_alternative<Core::null>(*ptr))
            {
                result.first = false;

                return result;
            }

            result.first = true;
            result.second = *ptr;

            return result;
        }

        result.second = param.param_type == Core::TypeParameterType::Type
                            ? nullptr
                            : Core::Value(Core::null{});

        return result;
    }

    TypeResolutionResult BaseType::resolve(AST::Type &type)
    {
        TypeResolutionResult result = {
            ResultStatus::Success,
            std::make_unique<Type>(this, std::vector<TypeArgument>{}),
            {}};

        bool is_generic = typeid(type) == typeid(AST::GenericType);

        if (parameters_.empty())
        {
            if (is_generic)
            {
                return result;
                // return TypeResolutionStatus::InvalidGenericUse;
            }

            return result;
        }

        if (!is_generic)
        {
            return result;
            // return TypeResolutionStatus::InvalidNonGenericUse;
        }

        AST::GenericType &n_type = static_cast<AST::GenericType &>(type);
        std::vector<std::unique_ptr<AST::Type>> &arguments = n_type.arguments();

        size_t arg_count = arguments.size();

        if (parameters_.size() != arg_count)
        {
            return result;
            // return TypeResolutionStatus::ArgumentCountMismatch;
        }

        result.data->arguments.reserve(arg_count);

        for (size_t i = 0; i < arg_count; i++)
        {
            std::unique_ptr<AST::Type> &argument = arguments[i];
            TypeParameter &param = parameters_[i];

            auto [succeeded, parsed] = resolve_argument(i, *argument);
            result.data->arguments.push_back(std::move(parsed));

            if (succeeded)
                continue;

            auto diagnostic = std::make_unique<Diagnostic::ErrorDiagnostic>(
                &type, Diagnostic::ErrorTypes::Type::InvalidTypeArgumentType,
                std::string("Invalid type argument \"") + Diagnostic::ERR_GEN +
                    type.to_string() + Utils::Styles::Reset +
                    "\" for type parameter \"" + Diagnostic::ERR_GEN +
                    std::string(param.name) + Utils::Styles::Reset +
                    ". Expected a " + Diagnostic::ERR_GEN +
                    (param.param_type == TypeParameterType::Type ? "TYPE"
                                                                 : "VALUE") +
                    Utils::Styles::Reset +
                    (param.type ? std::string(" assignable to ") +
                                      Diagnostic::ERR_GEN +
                                      std::string(param.type->type->name_) +
                                      Utils::Styles::Reset
                                : "") +
                    ".",
                "Provided type argument here");

            if (param.param_type == TypeParameterType::Constant)
                diagnostic->add_detail(param.type->make_suggestion(&type));

            result.error(std::move(diagnostic));

            return result;
        }

        return result;
    }

    bool BaseType::assignable_with(const BaseType &type) const
    {
        return name_ == type.name_;
    }

} // namespace Core
