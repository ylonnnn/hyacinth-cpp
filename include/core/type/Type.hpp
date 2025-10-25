#pragma once

#include <string>

#include "core/environment/Environment.hpp"
#include "core/result/Result.hpp"
#include "core/type/TypeParameter.hpp"
#include "core/value/Value.hpp"

namespace Core
{
    struct InstantiatedType;

    using TypeArgument = std::variant<InstantiatedType *, Value *>;

    struct TypeResult : Result<InstantiatedType *>
    {
        InstantiatedType *data = nullptr;

        TypeResult(ResultStatus status, InstantiatedType *data,
                   Diagnostic::DiagnosticList &&diagnostics);
    };

    struct BaseType
    {
        Environment &environment;
        std::string name;

        BaseType(Environment &environment, std::string &&name);

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
