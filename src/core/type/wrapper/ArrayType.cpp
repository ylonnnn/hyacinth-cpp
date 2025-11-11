#include <cassert>

#include "core/type/Type.hpp"
#include "core/type/TypePool.hpp"
#include "core/type/wrapper/ArrayType.hpp"
#include "utils/dev.hpp"

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

        TypeResult result{ResultStatus::Success,
                          const_cast<InstantiatedType *>(
                              static_cast<const InstantiatedType *>(this)),
                          {}};

        ArrayType::Signal signal = base.assignable(arguments, value, result);
        if (value == nullptr)
            return result;

        PositionRange *range = value->range;
        ReadValue &rval = get_rvalue(*value);

        if (signal == ArrayType::Mismatch)
        {
            result
                .error(*range, Diagnostic::ErrorType::TypeMismatch,
                       "expected value of type '" + *to_string() +
                           "', received '" + rval.type->to_string() + "'.")
                ->add_detail(base.make_suggestion(arguments, value));

            return result;
        }

        TypeResult t_res =
            ArrayInstantiated::assignable(*rval.type, value->range);
        result.adapt(t_res.status, std::move(t_res.diagnostics));

        return result;
    }

    TypeResult ArrayInstantiated::assignable(const InstantiatedType &type,
                                             PositionRange *range) const
    {
        if (range == nullptr)
            range = type.range;

        TypeResult result{ResultStatus::Success,
                          const_cast<InstantiatedType *>(
                              static_cast<const InstantiatedType *>(this)),
                          {}};

        if (typeid(type) != typeid(ArrayInstantiated))
        {
            result.error(*range, Diagnostic::ErrorType::TypeMismatch,
                         "expected value of type '" + *to_string() +
                             "', received '" + *type.to_string() + "'.");

            return result;
        }

        const GenericArgument &t_arg = arguments[0],
                              &tt_arg = type.arguments[0];

        // NOTE: std::get_if<>() returns a pointer and it can be null. However,
        // at this point, it is guaranteed that it will not be a nullptr due to
        // argument validation

        auto ptr = std::get_if<InstantiatedType *>(&t_arg),
             t_ptr = std::get_if<InstantiatedType *>(&tt_arg);
        InstantiatedType *ty = *ptr, *tty = *t_ptr;

        return ty->assignable(*tty, tty->range);
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
        if (value == nullptr)
            return Mismatch;

        ReadValue &rval = get_rvalue(*value);
        auto v_ptr = std::get_if<array>(rval.value.get());
        if (v_ptr == nullptr) [[unlikely]]
            return Mismatch;

        Core::array &val = *v_ptr;

        const GenericArgument &t_arg = arguments[0];
        auto ptr = std::get_if<InstantiatedType *>(&t_arg);
        if (ptr == nullptr) [[unlikely]]
            return Mismatch;

        InstantiatedType *type = *ptr;
        for (size_t i = 0; i < val.size(); ++i)
        {
            Value *el = val.get(i);
            if (el == nullptr)
                return Mismatch;

            TypeResult el_res = type->assignable(el);
            result.adapt(el_res.status, std::move(el_res.diagnostics));
        }

        return Assignable;
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
