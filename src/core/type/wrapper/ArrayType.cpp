#include "core/type/wrapper/ArrayType.hpp"
#include "core/type/Type.hpp"
#include "core/type/TypePool.hpp"
#include "utils/dev.hpp"
#include <cassert>

namespace Core
{
    ArrayInstantiated::ArrayInstantiated(
        BaseType &base, std::vector<GenericArgument> &&arguments,
        Core::PositionRange *range)
        : InstantiatedType(base, std::move(arguments), range)
    {
    }

    TypeResult ArrayInstantiated::assignable(Value *value) const
    {
        utils::todo("implement ArrayInstantiated::assignable()");

        TypeResult result{ResultStatus::Success, nullptr, {}};

        return result;
    }

    ArrayType *ArrayType::instance(Environment *environment)
    {
        if (instance_ != nullptr)
            return instance_.get();

        assert(environment != nullptr);
        instance_ = std::make_unique<ArrayType>(*environment);

        return instance_.get();
    }

    ArrayType::ArrayType(Environment &environment)
        : BaseType(environment, "array")
    {
        add_parameter(TypeParameterType::Type, "T", nullptr);
        // TODO: Add N Parameter
    }

    ArrayType::~ArrayType() = default;

    void ArrayType::default_operations() {}

    ArrayType::T *
    ArrayType::create_instance(std::vector<GenericArgument> &&arguments,
                               Core::PositionRange *range)
    {
        return TYPE_POOL.add(
            std::make_unique<T>(*this, std::move(arguments), range));
    }

    ArrayType::Signal
    ArrayType::assignable(const std::vector<GenericArgument> &arguments,
                          Value *value, TypeResult &result) const
    {
        return 0;
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

    //

    std::unique_ptr<ArrayType> ArrayType::instance_ = nullptr;

} // namespace Core
