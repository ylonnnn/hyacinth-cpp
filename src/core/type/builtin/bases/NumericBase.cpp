#include "core/type/builtin/bases/NumericBase.hpp"
#include "core/type/Type.hpp"

namespace Core
{
    NumericInstantiated::NumericInstantiated(
        BaseType &base, std::vector<GenericArgument> &&arguments,
        Core::PositionRange *range)
        : InstantiatedType(base, std::move(arguments), range)
    {
    }

    TypeResult NumericInstantiated::assignable(const InstantiatedType &type,
                                               PositionRange *range) const
    {
        if (range == nullptr)
            range = type.range;

        TypeResult result = InstantiatedType::assignable(type, range);

        const GenericArgument &bw_arg = arguments[0],
                              &t_bw_arg = type.arguments[0];

        auto ptr = std::get_if<Value *>(&bw_arg),
             t_ptr = std::get_if<Value *>(&t_bw_arg);
        if (ptr == nullptr || t_ptr == nullptr) [[unlikely]]
            return result;

        ReadValue &rv = get_rvalue(**ptr), &t_rv = get_rvalue(**t_ptr);
        auto rv_ptr = std::get_if<integer>(rv.value.get()),
             t_rv_ptr = std::get_if<integer>(t_rv.value.get());
        if (rv_ptr == nullptr || t_rv_ptr == nullptr) [[unlikely]]
            return result;

        uint64_t bw = rv_ptr->as<uint64_t>(), t_bw = t_rv_ptr->as<uint64_t>();
        if (t_bw > bw)
            result.error(*range, Diagnostic::ErrorType::TypeMismatch,
                         "expected value of type '" + *to_string() +
                             "', received '" + *type.to_string() + "'.");

        return result;
    }

    NumericBase::NumericBase(Core::Environment &environment, std::string &&name)
        : BaseType(environment, std::move(name))
    {
    }

    // Type *NumericBase::higher_bit_width(Type *a, Type *b) const
    // {
    //     if (a == nullptr || b == nullptr)
    //         return nullptr;

    //     const GenericArgument &bw = a->arguments[0], &bw_ = b->arguments[0];
    //     if (bw.valueless_by_exception() || bw_.valueless_by_exception())
    //         return nullptr;

    //     return std::visit(
    //         [&](auto &val, auto &val_) -> Type *
    //         {
    //             using Ty = std::decay_t<decltype(val)>;
    //             using Ty_ = std::decay_t<decltype(val_)>;

    //             if constexpr (std::is_same_v<Ty,
    //                                          std::shared_ptr<Core::Value>> &&
    //                           std::is_same_v<Ty_,
    //                           std::shared_ptr<Core::Value>>)

    //                 return std::visit(
    //                     [&](auto &v, auto &v_) -> Type *
    //                     {
    //                         using T = std::decay_t<decltype(v)>;
    //                         using T_ = std::decay_t<decltype(v_)>;

    //                         if constexpr (std::is_same_v<T, Core::h_int> &&
    //                                       std::is_same_v<T_, Core::h_int>)
    //                         {
    //                             return construct_wrapper(
    //                                 std::max(v.u64(), v_.u64()));
    //                         }

    //                         else
    //                             return nullptr;
    //                     },
    //                     *val, *val_);

    //             else
    //                 return nullptr;
    //         },
    //         bw, bw_);
    // }

} // namespace Core
