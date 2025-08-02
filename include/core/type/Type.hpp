#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "ast/type/Type.hpp"
#include "core/result/Result.hpp"
#include "core/symbol/type/TypeSymbol.hpp"
#include "core/value/Value.hpp"
#include "diagnostic/Diagnostic.hpp"
#include "diagnostic/NoteDiagnostic.hpp"

namespace Core
{
    class BaseType;
    class Type;
    class Environment;

    using TypeArgument = std::variant<Type, Core::Value>;

    class Type
    {
      public:
        BaseType *type;
        std::vector<TypeArgument> arguments;

        Type(BaseType *type, std::vector<TypeArgument> arguments);

        bool assignable(const Core::Value &value) const;
        virtual bool assignable_with(const Type &type) const;

        std::unique_ptr<Diagnostic::NoteDiagnostic>
        make_suggestion(AST::Node *node) const;

        std::string to_string() const;
    };

    enum class TypeParameterType
    {
        Type,
        Constant,
    };

    struct TypeParameter
    {
        std::string_view name;
        TypeParameterType param_type = TypeParameterType::Type;
        std::optional<Type> type = std::nullopt;

        TypeParameter(std::string_view name, TypeParameterType param_type,
                      std::optional<Type> type = std::nullopt);

        TypeArgument resolve(Environment &environment, AST::Type &type);
    };

    struct TypeResolutionResult : public Result<std::unique_ptr<Type>>
    {
        TypeResolutionResult(ResultStatus status, std::unique_ptr<Type> data,
                             Diagnostic::DiagnosticList diagnostics);
    };

    class BaseType
    {
      protected:
        Environment *environment_;
        std::string_view name_;
        std::vector<TypeParameter> parameters_;

        TypeSymbol *symbol_ = nullptr;

        bool builtin_ = false;

      public:
        BaseType(Environment *environment, std::string_view name,
                 TypeSymbol *symbol = nullptr);

        virtual ~BaseType();

      protected:
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

        void create_parameter(std::string_view name,
                              TypeParameterType param_type,
                              std::optional<Type> type = std::nullopt);
        std::pair<bool, TypeArgument> resolve_argument(size_t param_idx,
                                                       AST::Type &type);

        virtual std::unique_ptr<Type> construct_wrapper() const;

        TypeResolutionResult resolve(AST::Type &type);

        virtual bool assignable_with(const BaseType &type) const;

        friend Type;
    };

    using TypeTable =
        std::unordered_map<std::string_view, std::unique_ptr<BaseType>>;

} // namespace Core
