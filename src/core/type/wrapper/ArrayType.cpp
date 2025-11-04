#include "core/type/wrapper/ArrayType.hpp"
#include "core/type/Type.hpp"
#include "core/type/TypePool.hpp"
#include "utils/dev.hpp"

namespace Core
{
    ArrayInstantiated::ArrayInstantiated(
        BaseType &base, std::vector<GenericArgument> &&arguments)
        : InstantiatedType(base, std::move(arguments))
    {
    }

    TypeResult ArrayInstantiated::assignable(Value *value) const
    {
        utils::todo("implement ArrayInstantiated::assignable()");

        TypeResult result{ResultStatus::Success, nullptr, {}};

        return result;
    }

    ArrayType::ArrayType(Environment &environment)
        : BaseType(environment, "array")
    {
    }

    ArrayType::~ArrayType() = default;

    void ArrayType::default_operations() {}

    ArrayType::T *
    ArrayType::create_instance(std::vector<GenericArgument> &&arguments)
    {
        return TYPE_POOL.add(std::make_unique<T>(*this, std::move(arguments)));
    }

    ArrayType::T *ArrayType::infer(Value &value)
    {
        utils::todo("implement ArrayInstantiated::infer()");

        return nullptr;
    }

    std::unique_ptr<Diagnostic::Diagnostic>
    ArrayType::make_suggestion(const std::vector<GenericArgument> &arguments,
                               Value *value) const
    {
        return nullptr;
    }

} // namespace Core
