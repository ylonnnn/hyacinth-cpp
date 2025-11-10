#include <cassert>

#include "core/type/Type.hpp"
#include "core/type/TypePool.hpp"
#include "core/type/wrapper/ReferenceType.hpp"
#include "utils/dev.hpp"

namespace Core
{
    ReferenceInstantiated::ReferenceInstantiated(
        BaseType &base, std::vector<GenericArgument> &&arguments,
        Core::PositionRange *range)
        : InstantiatedType(base, std::move(arguments), range)
    {
    }

    TypeResult ReferenceInstantiated::assignable(Value *value) const
    {
        utils::todo("implement ReferenceInstantiated::assignable()");

        TypeResult result{ResultStatus::Success, nullptr, {}};

        return result;
    }

    ReferenceType *ReferenceType::instance(Environment *environment)
    {
        if (instance_ != nullptr)
            return instance_.get();

        assert(environment != nullptr);
        instance_ = std::make_unique<ReferenceType>(*environment);

        return instance_.get();
    }

    ReferenceType::ReferenceType(Environment &environment)
        : BaseType(environment, "array")
    {
        add_parameter(TypeParameterType::Type, "T", nullptr);
    }

    ReferenceType::~ReferenceType() = default;

    void ReferenceType::default_operations() {}

    ReferenceType::T *
    ReferenceType::create_instance(std::vector<GenericArgument> &&arguments,
                                   Core::PositionRange *range)
    {
        return TYPE_POOL.add(
            std::make_unique<T>(*this, std::move(arguments), range));
    }

    ReferenceType::Signal
    ReferenceType::assignable(const std::vector<GenericArgument> &arguments,
                              Value *value, TypeResult &result) const
    {
        return 0;
    }

    ReferenceType::T *ReferenceType::infer(Value &value)
    {
        utils::todo("implement ReferenceInstantiated::infer()");

        return nullptr;
    }

    std::unique_ptr<Diagnostic::Diagnostic> ReferenceType::make_suggestion(
        const std::vector<GenericArgument> &arguments, Value *value) const
    {
        return nullptr;
    }

    //

    std::unique_ptr<ReferenceType> ReferenceType::instance_ = nullptr;

} // namespace Core
