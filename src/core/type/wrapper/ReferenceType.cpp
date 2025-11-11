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

        TypeResult result{ResultStatus::Success,
                          const_cast<InstantiatedType *>(
                              static_cast<const InstantiatedType *>(this)),
                          {}};

        if (value == nullptr)
            return result;

        const GenericArgument &t_arg = arguments[0];
        auto ty = *std::get_if<InstantiatedType *>(&t_arg);

        if (typeid(*value) == typeid(ReadValue))
        {
            result.error(*value->range, Diagnostic::ErrorType::UnexpectedRValue,
                         "expects an l-value reference of type '" +
                             ty->to_string() + "'.");

            return result;
        }

        ReadValue &rval = get_rvalue(*value);

        return ReferenceInstantiated::assignable(*rval.type, rval.range);
    }

    TypeResult ReferenceInstantiated::assignable(const InstantiatedType &type,
                                                 PositionRange *range) const
    {
        TypeResult result{ResultStatus::Success,
                          const_cast<InstantiatedType *>(
                              static_cast<const InstantiatedType *>(this)),
                          {}};

        const GenericArgument &t_arg = arguments[0],
                              &tt_arg = type.arguments[0];

        // NOTE: std::get_if<>() returns a pointer and it can be null. However,
        // at this point, it is guaranteed that it will not be a nullptr due to
        // argument validation

        auto ptr = std::get_if<InstantiatedType *>(&t_arg);
        InstantiatedType *ty = *ptr,
                         *tty = const_cast<InstantiatedType *>(&type);

        if (typeid(type) == typeid(Core::ReferenceInstantiated))
            tty = *std::get_if<InstantiatedType *>(&tt_arg);

        if (ty->to_string() == tty->to_string())
            return result;

        // TODO: Polymorphic Reference Check

        result.error(*range, Diagnostic::ErrorType::TypeMismatch,
                     "expected value of type '&" + ty->to_string() +
                         "', received '" + *type.to_string() + "'.");

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
        : BaseType(environment, "reference")
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

    Value *ReferenceType::transfer_semantics(Value *value) const
    {
        return value; // References refer to the same value
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
