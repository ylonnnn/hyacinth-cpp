#pragma once

#include <string>

#include "core/argument/Argument.hpp"
#include "core/result/Result.hpp"
#include "core/type/TypeParameter.hpp"
#include "core/type/TypePool.hpp"

namespace Core
{
    struct TypeResult : Result<InstantiatedType *>
    {
        TypeResult(ResultStatus status, InstantiatedType *data,
                   Diagnostic::DiagnosticList &&diagnostics);
    };

    struct BaseType
    {
        using T = InstantiatedType;
        using Signal = uint32_t;

        Environment &environment;
        std::string name;

        std::vector<TypeParameter> parameters;

        BaseType(Environment &environment, std::string &&name);
        virtual ~BaseType() = default;

        virtual void default_operations();

        virtual T *create_instance(std::vector<GenericArgument> &&arguments);

        void add_parameter(TypeParameterType param_type, std::string &&name,
                           InstantiatedType *type);

        virtual Signal assignable(const std::vector<GenericArgument> &arguments,
                                  Value *value, TypeResult &result) const = 0;

        size_t hash();
        size_t *hash() const;

      private:
        std::pair<bool, size_t> hash_info_;
    };

    struct InstantiatedType
    {
        BaseType &base;
        std::vector<GenericArgument> arguments;

        InstantiatedType(BaseType &base,
                         std::vector<GenericArgument> &&arguments);

        virtual TypeResult assignable(Value *value) const;

        size_t hash(bool rehash = false);
        size_t *hash() const;

        std::string &to_string(bool restr = false);
        std::string *to_string() const;

      private:
        std::pair<bool, size_t> hash_info_;
        std::pair<bool, std::string> str_info_;
    };

} // namespace Core
