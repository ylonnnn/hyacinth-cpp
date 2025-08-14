#include <functional>
#include <string>
#include <variant>

#include "ast/type/GenericType.hpp"
#include "core/environment/Environment.hpp"
#include "core/result/Result.hpp"
#include "core/type/Type.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "utils/value.hpp"

namespace Core
{
    std::unordered_map<std::string, std::unique_ptr<Type>> Type::pool_{};

    Type::Type(BaseType *type, std::vector<TypeArgument> arguments)
        : type(type), arguments(std::move(arguments)),
          base_(this->arguments.empty() ? nullptr
                                        : Type::get_or_create(type, {}))
    {
        if (type != nullptr)
            signature_ = to_string();
    }

    Type *Type::base() { return base_; }

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

    Type *Type::from_value(Core::Environment *environment,
                           const Core::Value &value)
    {
        if (environment == nullptr)
            return nullptr;

        return std::visit(
            [&](const auto &val) -> Type *
            {
                using T = std::decay_t<decltype(val)>;
                std::string type_name;

                if constexpr (std::is_same_v<T, Core::h_int>)
                    type_name = val.is_signed() ? "int" : "uint";

                else if constexpr (std::is_same_v<T, double>)
                    type_name = "float";

                else if constexpr (std::is_same_v<T, bool>)
                    type_name = "bool";

                else if constexpr (std::is_same_v<T, char>)
                    type_name = "char";

                else if constexpr (std::is_same_v<T, std::string>)
                    type_name = "str";

                else if constexpr (std::is_same_v<T, std::string>)
                    type_name = "void";

                BaseType *resolved = environment->resolve_type(type_name);
                if (resolved == nullptr)
                    return nullptr;

                return resolved->from_value(val);
            },
            value);
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

    size_t Type::hash() const { return std::hash<std::string>{}(signature_); }

    TypeParameter::TypeParameter(std::string_view name,
                                 TypeParameterType param_type, Type *type)
        : name(name), param_type(type ? param_type : TypeParameterType::Type),
          type(std::move(type))
    {
    }

    TypeResolutionResult::TypeResolutionResult(
        ResultStatus status, Type *data, Diagnostic::DiagnosticList diagnostics)
        : Core::Result<Type *>(status, data, std::move(diagnostics))
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
                if (resolved == nullptr || this->type == nullptr)
                    return Type(resolved, {});

                return Type(this->type->type->assignable_with(*resolved)
                                ? resolved
                                : nullptr,
                            {});
            }

            case Core::TypeParameterType::Constant:
            {
                Core::Value parsed = Utils::parse_val(type.value());
                if (this->type == nullptr)
                    return Core::null{};

                return this->type->assignable(parsed) ? parsed : Core::null{};
            }
        }

        return nullptr;
    }

    TypeMember::TypeMember(std::variant<Type *, FunctionSymbol *> &&value,
                           TypeMemberAccessibility accessibility)
        : accessibility_(accessibility), value_(std::move(value))
    {
    }

    bool TypeMember::is_field() const
    {
        return std::holds_alternative<Type *>(value_);
    }

    bool TypeMember::is_function() const
    {
        return std::holds_alternative<FunctionSymbol *>(value_);
    }

    BaseType::BaseType(Environment *environment, std::string_view name,
                       TypeSymbol *symbol)
        : environment_(environment), name_(name), symbol_(symbol)
    {
        parameters_.reserve(8);
    }

    BaseType::~BaseType()
    {
        // std::cout << "destroyed: " << name_ << " | " << this << "\n";
    }

    std::string_view BaseType::name() const { return name_; }

    std::vector<TypeParameter> &BaseType::parameters() { return parameters_; }

    TypeSymbol *BaseType::symbol() { return symbol_; }

    std::unordered_map<std::string, TypeMember> &BaseType::members()
    {
        return members_;
    }

    const std::unordered_map<std::string, TypeMember> &BaseType::members() const
    {
        return members_;
    }

    void BaseType::add_parameter(TypeParameter &&parameter)
    {
        parameters_.push_back(std::move(parameter));
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

    Type *BaseType::construct_wrapper() const
    {
        return Type::get_or_create<>(const_cast<BaseType *>(this), {});
    }

    TypeResolutionResult BaseType::resolve(AST::Type &type)
    {
        TypeResolutionResult result = {
            ResultStatus::Success, construct_wrapper(), {}};

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

        std::vector<TypeArgument> _arguments{};
        _arguments.reserve(arg_count);

        for (size_t i = 0; i < arg_count; i++)
        {
            std::unique_ptr<AST::Type> &argument = arguments[i];
            TypeParameter &param = parameters_[i];

            auto [succeeded, parsed] = resolve_argument(i, *argument);
            _arguments.push_back(std::move(parsed));

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

        result.data = Type::get_or_create<>(const_cast<BaseType *>(this),
                                            std::move(_arguments));

        return result;
    }

    bool BaseType::assignable_with(const BaseType &type) const
    {
        return name_ == type.name_;
    }

} // namespace Core
