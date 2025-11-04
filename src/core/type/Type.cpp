#include <type_traits>
#include <variant>

#include "core/environment/Environment.hpp"
#include "core/type/Type.hpp"

namespace Core
{
    TypeResult::TypeResult(ResultStatus status, InstantiatedType *data,
                           Diagnostic::DiagnosticList &&diagnostics)
        : Result<InstantiatedType *>(status, data, std::move(diagnostics))
    {
    }

    BaseType::BaseType(Environment &environment, std::string &&name)
        : environment(environment), name(std::move(name))
    {
        hash();

        default_operations();
    }

    void BaseType::default_operations()
    {
        // To allow implementations with no default operations as leaving this
        // as a pure virtual turns derived classes to virtual when a virtual
        // method is not overriden
    }

    BaseType::T *
    BaseType::create_instance(std::vector<GenericArgument> &&arguments)
    {
        return TYPE_POOL.add<T>(
            std::make_unique<T>(*this, std::move(arguments)));
    }

    void BaseType::add_parameter(TypeParameterType param_type,
                                 std::string &&name, InstantiatedType *type)
    {
        parameters.emplace_back(param_type, std::move(name), type);
    }

    size_t BaseType::hash()
    {
        if (hash_info_.first)
            return hash_info_.second;

        return hash_info_.first = true,
               hash_info_.second = std::hash<std::string>{}(name);
    }

    size_t *BaseType::hash() const
    {
        if (!hash_info_.first)
            return nullptr;

        return const_cast<size_t *>(&hash_info_.second);
    }

    BaseType::T *BaseType::infer(Environment &environment, Value &value)
    {
        if (value.type != nullptr)
            return value.type;

        if (value.value == nullptr)
            return nullptr;

        return std::visit(
            [&environment, &value](auto &val) -> T *
            {
                using T = std::decay_t<decltype(val)>;
                std::string t_name;

                if constexpr (std::is_same_v<T, Core::integer>)
                    t_name = val.is_negative ? "int" : "uint";

                else if constexpr (std::is_same_v<T, double>)
                    t_name = "float";

                BaseType *resolved = environment.resolve_type(t_name);
                if (resolved == nullptr)
                    return nullptr;

                return resolved->infer(value);
            },
            *value.value);
    }

    InstantiatedType::InstantiatedType(BaseType &base,
                                       std::vector<GenericArgument> &&arguments)
        : base(base), arguments(std::move(arguments))
    {
        hash();
        to_string();
    }

    TypeResult InstantiatedType::assignable(Value *value) const
    {
        TypeResult result{ResultStatus::Success, nullptr, {}};
        base.assignable(arguments, value, result);

        return result;
    }

    size_t InstantiatedType::hash(bool rehash)
    {
        if (rehash || !hash_info_.first)
        {
            size_t hash_val = base.hash();

            for (const auto &argument : arguments)
            {
                if (auto ptr = std::get_if<InstantiatedType *>(&argument))
                    hash_val += (*ptr)->hash();

                else if (auto ptr = std::get_if<Value *>(&argument))
                {
                    std::visit(
                        [&hash_val](const auto &val) -> void
                        {
                            using T = std::decay_t<decltype(val)>;
                            if constexpr (std::is_fundamental_v<T>)
                                hash_val += std::hash<T>{}(val);
                            else if constexpr (std::is_base_of_v<
                                                   value_base_type, T>)
                                hash_val += val.hash();
                        },
                        *(*ptr)->value);
                }
            }

            return hash_info_.first = true, hash_info_.second = hash_val;
        }

        return hash_info_.second;
    }

    size_t *InstantiatedType::hash() const
    {
        if (!hash_info_.first)
            return nullptr;

        return const_cast<size_t *>(&hash_info_.second);
    }

    std::string &InstantiatedType::to_string(bool restr)
    {
        if (restr || !str_info_.first)
        {
            str_info_.first = true;
            str_info_.second = base.name;

            std::string &str = str_info_.second;

            if (arguments.empty())
                return str;

            str += "<";

            for (auto &argument : arguments)
            {
                if (auto ptr = std::get_if<Value *>(&argument))
                {
                    std::visit(
                        [&](auto &val) -> void
                        {
                            using T = std::decay_t<decltype(val)>;

                            if constexpr (std::is_convertible_v<T, std::string>)
                                str += std::string(val);
                            else
                                str += std::to_string(val);
                        },
                        *(*ptr)->value);
                }

                else if (auto ptr = std::get_if<InstantiatedType *>(&argument))
                {
                    auto type = *ptr;
                    str += type == nullptr ? "" : type->to_string();
                }

                if (argument != arguments.back())
                    str += ", ";
            }

            return str += ">";
        }

        return str_info_.second;
    }

    std::string *InstantiatedType::to_string() const
    {
        if (!str_info_.first)
            return nullptr;

        return const_cast<std::string *>(&str_info_.second);
    }

    // Type::Type(BaseType *base, std::vector<GenericArgument> &&arguments)
    //     : base(base), arguments(std::move(arguments))
    // {
    //     if (base != nullptr)
    //         signature = to_string();
    // }

    // std::string Type::to_string() const
    // {
    // }

} // namespace Core

// namespace Core
// {
//     bool Type::assignable(const Core::Value &value) const
//     {
//         return type->assignable(value, arguments);
//     }

//     bool Type::assignable_with(const Type &type) const
//     {
//         if (type.type == nullptr)
//             return false;

//         return this->type->assignable_with(*type.type);
//     }

//     std::unique_ptr<Diagnostic::NoteDiagnostic>
//     Type::make_suggestion(AST::Node *node) const
//     {
//         return type->make_suggestion(node, arguments);
//     }

//     Type *Type::from_value(Core::Environment *environment,
//                            const Core::Value &value)
//     {
//         if (environment == nullptr)
//             return nullptr;

//         return std::visit(
//             [&](const auto &val) -> Type *
//             {
//                 using T = std::decay_t<decltype(val)>;
//                 std::string type_name;

//                 if constexpr (std::is_same_v<T, h_int>)
//                     type_name = val.is_signed() ? "int" : "uint";

//                 else if constexpr (std::is_same_v<T, double>)
//                     type_name = "float";

//                 else if constexpr (std::is_same_v<T, bool>)
//                     type_name = "bool";

//                 else if constexpr (std::is_same_v<T, char>)
//                     type_name = "char";

//                 else if constexpr (std::is_same_v<T, std::string>)
//                     type_name = "str";

//                 else if constexpr (std::is_same_v<T, std::string>)
//                     type_name = "void";

//                 else if constexpr (std::is_same_v<T, object>)
//                     type_name = val.type()->type->name();

//                 else if constexpr (std::is_same_v<T, array>)
//                     type_name = "array";

//                 BaseType *resolved = environment->resolve_type(type_name);
//                 if (resolved == nullptr)
//                     return nullptr;

//                 return resolved->from_value(val);
//             },
//             value);
//     }

//     TypeParameter::TypeParameter(std::string_view name,
//                                  TypeParameterType param_type, Type *type)
//         : name(name), param_type(type ? param_type :
//         TypeParameterType::Type),
//           type(std::move(type))
//     {
//     }

//     TypeResolutionResult::TypeResolutionResult(
//         ResultStatus status, Type *data, Diagnostic::DiagnosticList
//         diagnostics) : Core::Result<Type *>(status, data,
//         std::move(diagnostics))
//     {
//     }

//     GenericArgument TypeParameter::resolve(Environment &environment,
//                                         AST::Type &type)
//     {
//         switch (param_type)
//         {
//             case Core::TypeParameterType::Type:
//             {
//                 BaseType *resolved =
//                     environment.resolve_type(std::string(type.value().value));
//                 if (resolved == nullptr)
//                     return Type::get_or_create(resolved, {});

//                 return resolved->resolve(type).data;
//             }

//             case Core::TypeParameterType::Constant:
//             {
//                 Core::Value parsed = utils::parse_val(type.value());
//                 if (this->type == nullptr)
//                     return std::make_shared<Core::Value>(Core::null{});

//                 return std::make_shared<Core::Value>(
//                     this->type->assignable(parsed) ? parsed : Core::null{});
//             }
//         }

//         return nullptr;
//     }

//     TypeMember::TypeMember(std::variant<Type *, FunctionSymbol *> &&value,
//                            TypeMemberAccessibility accessibility)
//         : accessibility_(accessibility), value_(std::move(value))
//     {
//     }

//     bool TypeMember::is_field() const
//     {
//         return std::holds_alternative<Type *>(value_);
//     }

//     bool TypeMember::is_function() const
//     {
//         return std::holds_alternative<FunctionSymbol *>(value_);
//     }

//     BaseType::BaseType(Environment *environment, std::string_view name,
//                        TypeSymbol *symbol)
//         : environment_(environment), name_(name), symbol_(symbol)
//     {
//         parameters_.reserve(8);
//     }

//     BaseType::~BaseType()
//     {
//         // std::cout << "destroyed: " << name_ << " | " << this << "\n";
//     }

//     std::string_view BaseType::name() const { return name_; }

//     std::vector<TypeParameter> &BaseType::parameters() { return parameters_;
//     }

//     TypeSymbol *BaseType::symbol() { return symbol_; }

//     std::unordered_map<std::string, TypeMember> &BaseType::members()
//     {
//         return members_;
//     }

//     const std::unordered_map<std::string, TypeMember> &BaseType::members()
//     const
//     {
//         return members_;
//     }

//     void BaseType::add_parameter(TypeParameter &&parameter)
//     {
//         parameters_.push_back(std::move(parameter));
//     }

//     std::pair<bool, GenericArgument> BaseType::resolve_argument(size_t
//     param_idx,
//                                                              AST::Type &type)
//     {
//         std::pair<bool, GenericArgument> result = {false, nullptr};

//         if (param_idx >= parameters_.size())
//             return result;

//         TypeParameter &param = parameters_[param_idx];
//         GenericArgument resolved = param.resolve(*environment_, type);

//         if (auto ptr = std::get_if<Type *>(&resolved))
//         {
//             if ((*ptr)->type == nullptr)
//                 return result;

//             result.first = true;
//             result.second = std::move(*ptr);

//             return result;
//         }

//         else if (auto ptr =
//                      std::get_if<std::shared_ptr<Core::Value>>(&resolved))
//         {
//             if (std::holds_alternative<Core::null>(**ptr))
//             {
//                 result.first = false;

//                 return result;
//             }

//             result.first = true;
//             result.second = *ptr;

//             return result;
//         }

//         result.second = param.param_type == Core::TypeParameterType::Type
//                             ? nullptr
//                             : std::make_shared<Core::Value>(Core::null{});

//         return result;
//     }

//     Type *
//     BaseType::construct_wrapper(std::vector<GenericArgument> &&arguments)
//     const
//     {
//         return Type::get_or_create<>(const_cast<BaseType *>(this),
//                                      std::move(arguments));
//     }

//     TypeResolutionResult BaseType::resolve(AST::Type &type)
//     {
//         TypeResolutionResult result = {
//             ResultStatus::Success, construct_wrapper({}), {}};

//         bool is_generic = typeid(type) == typeid(AST::GenericType);

//         if (parameters_.empty())
//         {
//             if (is_generic)
//             {
//                 return result;
//                 // return TypeResolutionStatus::InvalidGenericUse;
//             }

//             return result;
//         }

//         size_t param_count = parameters_.size();

//         if (!is_generic)
//         {
//             result.error(Diagnostic::create_invalid_arguments_error(
//                 &type, param_count, 0));

//             return result;
//             // return TypeResolutionStatus::InvalidNonGenericUse;
//         }

//         AST::GenericType &n_type = static_cast<AST::GenericType &>(type);
//         std::vector<std::unique_ptr<AST::Type>> &arguments =
//         n_type.arguments();

//         size_t arg_count = arguments.size();

//         if (param_count != arg_count)
//         {
//             return result;
//             // return TypeResolutionStatus::ArgumentCountMismatch;
//         }

//         std::vector<GenericArgument> _arguments{};
//         _arguments.reserve(arg_count);

//         for (size_t i = 0; i < arg_count; i++)
//         {
//             std::unique_ptr<AST::Type> &argument = arguments[i];
//             TypeParameter &param = parameters_[i];

//             auto [succeeded, parsed] = resolve_argument(i, *argument);
//             _arguments.push_back(std::move(parsed));

//             if (succeeded)
//                 continue;

//             auto diagnostic = std::make_unique<Diagnostic::ErrorDiagnostic>(
//                 &type,
//                 Diagnostic::ErrorTypes::Type::InvalidGenericArgumentType,
//                 std::string("Invalid type argument \"") + Diagnostic::ERR_GEN
//                 +
//                     type.to_string() + utils::Styles::Reset +
//                     "\" for type parameter \"" + Diagnostic::ERR_GEN +
//                     std::string(param.name) + utils::Styles::Reset +
//                     "\". Expected a " + Diagnostic::ERR_GEN +
//                     (param.param_type == TypeParameterType::Type ? "TYPE"
//                                                                  : "VALUE") +
//                     utils::Styles::Reset +
//                     (param.type ? std::string(" assignable to ") +
//                                       Diagnostic::ERR_GEN +
//                                       std::string(param.type->type->name_) +
//                                       utils::Styles::Reset
//                                 : "") +
//                     ".",
//                 "Provided type argument here");

//             if (param.param_type == TypeParameterType::Constant)
//                 diagnostic->add_detail(param.type->make_suggestion(&type));

//             result.error(std::move(diagnostic));

//             return result;
//         }

//         result.data = construct_wrapper(std::move(_arguments));

//         return result;
//     }

//     bool BaseType::assignable_with(const BaseType &type) const
//     {
//         return name_ == type.name_;
//     }

// } // namespace Core
