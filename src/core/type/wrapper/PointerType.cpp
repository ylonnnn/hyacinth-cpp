#include <cassert>

#include "core/type/Type.hpp"
#include "core/type/TypePool.hpp"
#include "core/type/wrapper/PointerType.hpp"
#include "utils/dev.hpp"

namespace Core
{
    PointerInstantiated::PointerInstantiated(
        BaseType &base, std::vector<GenericArgument> &&arguments,
        Core::PositionRange *range)
        : InstantiatedType(base, std::move(arguments), range)
    {
    }

    TypeResult PointerInstantiated::assignable(Value *value) const
    {
        utils::todo("implement PointerInstantiated::assignable()");

        TypeResult result{ResultStatus::Success, nullptr, {}};

        return result;
    }

    PointerType *PointerType::instance(Environment *environment)
    {
        if (instance_ != nullptr)
            return instance_.get();

        assert(environment != nullptr);
        instance_ = std::make_unique<PointerType>(*environment);

        return instance_.get();
    }

    PointerType::PointerType(Environment &environment)
        : BaseType(environment, "array")
    {
        add_parameter(TypeParameterType::Type, "T", nullptr);
    }

    PointerType::~PointerType() = default;

    void PointerType::default_operations() {}

    PointerType::T *
    PointerType::create_instance(std::vector<GenericArgument> &&arguments,
                                 Core::PositionRange *range)
    {
        return TYPE_POOL.add(
            std::make_unique<T>(*this, std::move(arguments), range));
    }

    PointerType::Signal
    PointerType::assignable(const std::vector<GenericArgument> &arguments,
                            Value *value, TypeResult &result) const
    {
        return 0;
    }

    PointerType::T *PointerType::infer(Value &value)
    {
        utils::todo("implement PointerInstantiated::infer()");

        return nullptr;
    }

    std::unique_ptr<Diagnostic::Diagnostic>
    PointerType::make_suggestion(const std::vector<GenericArgument> &arguments,
                                 Value *value) const
    {
        return nullptr;
    }

    //

    std::unique_ptr<PointerType> PointerType::instance_ = nullptr;

} // namespace Core
