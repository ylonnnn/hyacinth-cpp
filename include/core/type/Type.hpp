#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "ast/type/Type.hpp"
#include "core/result/Result.hpp"
#include "core/value/Value.hpp"
#include "diagnostic/Diagnostic.hpp"
#include "diagnostic/NoteDiagnostic.hpp"

namespace Core
{
    class Type;
    class Environment;

    using TypeArgument = std::variant<Type *, Core::Value>;

    enum class TypeParameterType
    {
        Type,
        Constant,
    };

    struct TypeParameter
    {
        std::string_view name;
        TypeParameterType param_type = TypeParameterType::Type;
        Type *type = nullptr;

        TypeArgument resolve(Environment &environment, AST::Type &type);
    };

    struct TypeResolutionResult : public Result<Type *>
    {
        std::vector<TypeArgument> arguments;

        TypeResolutionResult(ResultStatus status, Type *data,
                             Diagnostic::DiagnosticList diagnostics);
    };

    struct TypeAssignabilityOptions
    {
        virtual ~TypeAssignabilityOptions() = default;
    };

    class Type
    {
      protected:
        Environment *environment_;
        std::string_view name_;
        std::vector<TypeParameter> parameters_;

      public:
        Type(Environment *environment, std::string_view name);

        Environment *environment();
        std::string_view name() const;
        std::vector<TypeParameter> &parameters();

        void create_parameter(std::string_view name,
                              TypeParameterType param_type,
                              Type *type = nullptr);
        std::pair<bool, TypeArgument> resolve_argument(size_t param_idx,
                                                       AST::Type &type);

        TypeResolutionResult resolve(AST::Type &type);

        virtual bool
        assignable(const Core::Value &value,
                   const std::vector<TypeArgument> &arguments) const = 0;
        virtual bool assignable_with(const Type &type) const = 0;

        virtual std::unique_ptr<Diagnostic::NoteDiagnostic>
        make_suggestion(AST::Node *node,
                        const std::vector<TypeArgument> &arguments) const = 0;
    };

    using TypeTable = std::unordered_map<std::string, std::unique_ptr<Type>>;

} // namespace Core
