#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "ast/type/Type.hpp"
#include "core/operation/Operation.hpp"
#include "core/result/Result.hpp"
#include "core/symbol/type/TypeSymbol.hpp"
#include "core/value/Value.hpp"
#include "diagnostic/Diagnostic.hpp"
#include "diagnostic/NoteDiagnostic.hpp"
#include "utils/value.hpp"

namespace Core
{
    class BaseType;
    class Type;
    class Environment;

    using TypeArgument = std::variant<Type, Core::Value>;

    enum class TypeParameterType
    {
        Type,
        Constant,
    };

    struct TypeParameter;

    struct TypeResolutionResult : public Result<Type *>
    {
        TypeResolutionResult(ResultStatus status, Type *data,
                             Diagnostic::DiagnosticList diagnostics);
    };

    using TypeTable =
        std::unordered_map<std::string_view, std::unique_ptr<BaseType>>;

    class Type
    {
      private:
        static std::unordered_map<std::string, std::unique_ptr<Type>> pool_;

      public:
        template <typename T = Type,
                  typename = std::enable_if_t<std::is_base_of_v<Type, T>>>
        static Type *get_or_create(BaseType *base,
                                   const std::vector<TypeArgument> &arguments);

        static Type *from_value(Core::Environment *environment,
                                const Core::Value &value);

        BaseType *type;
        std::vector<TypeArgument> arguments;

      protected:
        Type *base_ = nullptr;
        std::string signature_;

      public:
        Type(BaseType *type, std::vector<TypeArgument> arguments);
        virtual ~Type() = default;

        Type *base();

        bool assignable(const Core::Value &value) const;
        virtual bool assignable_with(const Type &type) const;

        std::unique_ptr<Diagnostic::NoteDiagnostic>
        make_suggestion(AST::Node *node) const;

        std::string to_string() const;
        size_t hash() const;
    };

    class BaseType
    {
      protected:
        bool builtin_ = false;

        Environment *environment_;
        std::string_view name_;
        std::vector<TypeParameter> parameters_;

        TypeSymbol *symbol_ = nullptr;

      public:
        BaseType(Environment *environment, std::string_view name,
                 TypeSymbol *symbol = nullptr);

        virtual ~BaseType();

      protected:
        template <typename L, typename R>
        Operation::BinaryHandler
        overload(std::function<Value(const L &a, const R &b)> &&handler,
                 const std::function<Type *(Type *, Type *)> &nv_handler)
        {
            return [&, handler = std::move(handler),
                    nv_handler](const Operation::Operand &left_op,
                                const Operation::Operand &right_op)
                       -> std::optional<Operation::Assignee>
            {
                auto l_val = left_op.value, r_val = right_op.value;
                auto left = l_val ? Utils::as<L>(*l_val) : nullptr,
                     right = r_val ? Utils::as<R>(*r_val) : nullptr;

                if (left == nullptr || right == nullptr)
                    return Operation::Assignee{
                        nv_handler(left_op.type, right_op.type), std::nullopt};

                auto result = handler(*left, *right);
                return Operation::Assignee{
                    Type::from_value(environment_, result), result};
            };
        }

        virtual bool
        assignable(const Core::Value &value,
                   const std::vector<TypeArgument> &arguments) const = 0;

        virtual std::unique_ptr<Diagnostic::NoteDiagnostic>
        make_suggestion(AST::Node *node,
                        const std::vector<TypeArgument> &arguments) const = 0;

      public:
        Environment *environment();
        std::string_view name() const;
        std::vector<TypeParameter> &parameters();

        TypeSymbol *symbol();

        constexpr inline bool builtin() const { return builtin_; }

        void add_parameter(TypeParameter &&parameter);
        std::pair<bool, TypeArgument> resolve_argument(size_t param_idx,
                                                       AST::Type &type);

        virtual Type *construct_wrapper() const;

        TypeResolutionResult resolve(AST::Type &type);

        virtual Type *from_value(const Core::Value &value) const = 0;

        virtual bool assignable_with(const BaseType &type) const;

        friend Type;
    };

    struct TypeParameter
    {
        std::string_view name;
        TypeParameterType param_type = TypeParameterType::Type;
        Type *type = nullptr;

        TypeParameter(std::string_view name, TypeParameterType param_type,
                      Type *type = nullptr);

        TypeArgument resolve(Environment &environment, AST::Type &type);
    };

    template <typename T, typename>
    Type *Type::get_or_create(BaseType *base,
                              const std::vector<TypeArgument> &arguments)
    {
        std::string str(base->name());

        if (!arguments.empty())
        {
            str += "[";

            for (const auto &argument : arguments)
            {
                if (auto ptr = std::get_if<Type>(&argument))
                    str += ptr->to_string();

                else if (auto ptr = std::get_if<Value>(&argument))

                    str += std::visit(
                        [&](const auto &val) -> std::string
                        {
                            using T_ = std::decay_t<decltype(val)>;

                            if constexpr (std::is_convertible_v<T_,
                                                                std::string>)
                                return std::string(val);
                            else
                                return std::to_string(val);
                        },
                        *ptr);
            }

            str += "]";
        }

        auto it = pool_.find(str);
        if (it != pool_.end())
            return it->second.get();

        auto type = std::make_unique<T>(base, std::move(arguments));
        return pool_.emplace(type->signature_, std::move(type))
            .first->second.get();
    }

} // namespace Core
