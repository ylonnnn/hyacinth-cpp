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

        static constexpr Signal Mismatch = 0, Assignable = 1;

        Environment &environment;
        std::string name;

        std::vector<TypeParameter> parameters;

        BaseType(Environment &environment, std::string &&name);
        virtual ~BaseType() = default;

        virtual void default_operations();

        virtual T *create_instance(std::vector<GenericArgument> &&arguments,
                                   PositionRange *range = nullptr);

        void add_parameter(TypeParameterType param_type, std::string &&name,
                           InstantiatedType *type);
        TypeResult
        validate_arguments(const std::vector<GenericArgument> &arguments,
                           const PositionRange &range);

        // Semantics
        virtual Value *transfer_semantics(Value *value) const;

        virtual Signal assignable(const std::vector<GenericArgument> &arguments,
                                  Value *value, TypeResult &result) const = 0;

        static T *infer(Environment &environment, Value &value);
        virtual T *infer(Value &value) = 0;

        virtual std::unique_ptr<Diagnostic::Diagnostic>
        make_suggestion(const std::vector<GenericArgument> &arguments,
                        Value *value) const = 0;

        size_t hash();
        size_t *hash() const;

      private:
        std::pair<bool, size_t> hash_info_;
    };

    struct InstantiatedType
    {
        BaseType &base;
        std::vector<GenericArgument> arguments;
        PositionRange *range = nullptr;

        InstantiatedType(BaseType &base,
                         std::vector<GenericArgument> &&arguments,
                         PositionRange *range = nullptr);

        virtual TypeResult assignable(Value *value) const;
        virtual TypeResult assignable(const InstantiatedType &type,
                                      PositionRange *range = nullptr) const;

        size_t hash(bool rehash = false);
        size_t *hash() const;

        std::string &to_string(bool restr = false);
        std::string *to_string() const;

      private:
        std::pair<bool, size_t> hash_info_;
        std::pair<bool, std::string> str_info_;
    };

} // namespace Core
