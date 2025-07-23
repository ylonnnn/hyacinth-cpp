#include <iostream>

#include "ast/type/GenericType.hpp"
#include "core/environment/Environment.hpp"
#include "core/result/Result.hpp"
#include "core/type/Type.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "utils/value.hpp"

namespace Core
{
    TypeResolutionResult::TypeResolutionResult(
        ResultStatus status, Type *data, Diagnostic::DiagnosticList diagnostics)
        : Core::Result<Type *>(status, data, std::move(diagnostics))
    {
    }

    std::variant<Type *, Core::Value>
    TypeParameter::resolve(Environment &environment, AST::Type &type)
    {
        switch (param_type)
        {
            case Core::TypeParameterType::Type:
                return environment.resolve_type(
                    std::string(type.value().value));

            case Core::TypeParameterType::Constant:
            {
                Core::Value parsed = Utils::parse_val(type.value());
                if (this->type != nullptr && this->type->assignable(parsed, {}))
                    return parsed;

                return Core::null{};
            }
        }

        return nullptr;
    }

    Type::Type(Environment *environment, std::string_view name)
        : environment_(environment), name_(name)
    {
        parameters_.reserve(8);
    }

    std::string_view Type::name() const { return name_; }

    std::vector<TypeParameter> &Type::parameters() { return parameters_; }

    void Type::create_parameter(std::string_view name,
                                TypeParameterType param_type, Type *type)
    {
        parameters_.push_back(TypeParameter{name, param_type, type});
    }

    std::pair<bool, TypeArgument> Type::resolve_argument(size_t param_idx,
                                                         AST::Type &type)
    {
        std::pair<bool, TypeArgument> result = {false, nullptr};

        if (param_idx >= parameters_.size())
            return result;

        TypeParameter &param = parameters_[param_idx];

        std::variant<Type *, Core::Value> resolved =
            param.resolve(*environment_, type);

        if (auto ptr = std::get_if<Type *>(&resolved))
        {
            result.first = assignable_with(**ptr);
            result.second = *ptr;

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

    TypeResolutionResult Type::resolve(AST::Type &type)
    {
        TypeResolutionResult result = {ResultStatus::Success, this, {}};
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

        result.arguments.reserve(arg_count);

        for (size_t i = 0; i < arg_count; i++)
        {
            std::unique_ptr<AST::Type> &arg = arguments[i];
            TypeParameter &param = parameters_[i];

            auto [succeeded, parsed] = resolve_argument(i, *arg);
            result.arguments.push_back(std::move(parsed));

            if (!succeeded)
            {
                AST::Type &type = *arg;

                result.error(
                    &type,
                    Diagnostic::ErrorTypes::Type::InvalidTypeArgumentType,
                    std::string("Invalid type argument \"") +
                        Diagnostic::ERR_GEN + type.to_string() +
                        Utils::Styles::Reset + "\" for type parameter \"" +
                        Diagnostic::ERR_GEN + std::string(param.name) +
                        Utils::Styles::Reset + ". Expected a " +
                        Diagnostic::ERR_GEN +
                        (param.param_type == TypeParameterType::Type
                             ? "TYPE"
                             : "VALUE") +
                        Utils::Styles::Reset + " assignable to " +
                        Diagnostic::ERR_GEN + std::string(param.type->name_) +
                        Utils::Styles::Reset + ".",
                    "Provided type argument here");

                return result;
                // return TypeResolutionStatus::InvalidTypeArgumentType;
            }
        }

        return result;
    }

    bool Type::assignable_with(const Type &type) const
    {
        return name_ == type.name_;
    }

} // namespace Core
