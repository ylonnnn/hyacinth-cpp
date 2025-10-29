#pragma once

#include <string>

#include "core/result/Result.hpp"
#include "core/type/TypeParameter.hpp"
#include "core/type/TypePool.hpp"
#include "core/value/Value.hpp"

namespace Core
{
    using TypeArgument = std::variant<InstantiatedType *, Value *>;

    struct TypeResult : Result<InstantiatedType *>
    {
        TypeResult(ResultStatus status, InstantiatedType *data,
                   Diagnostic::DiagnosticList &&diagnostics);
    };

    struct BaseType
    {
        using T = InstantiatedType;

        Environment &environment;
        std::string name;

        std::vector<TypeParameter> parameters;

        BaseType(Environment &environment, std::string &&name);

        virtual void default_operations();

        virtual T *create_instance(std::vector<TypeArgument> &&arguments);

        void add_parameter(TypeParameterType param_type, std::string &&name,
                           InstantiatedType *type);

        virtual TypeResult
        assignable(const std::vector<TypeArgument> &arguments,
                   Value *value) const = 0;

        size_t hash();
        size_t *hash() const;

      private:
        std::pair<bool, size_t> hash_info_;
    };

    struct InstantiatedType
    {
        BaseType &base;
        std::vector<TypeArgument> arguments;

        InstantiatedType(BaseType &base, std::vector<TypeArgument> &&arguments);

        TypeResult assignable(Value *value) const;

        size_t hash(bool rehash = false);
        size_t *hash() const;

        std::string &to_string(bool restr = false);
        std::string *to_string() const;

      private:
        std::pair<bool, size_t> hash_info_;
        std::pair<bool, std::string> str_info_;
    };

} // namespace Core
