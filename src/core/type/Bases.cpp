#include "core/type/Bases.hpp"
#include <algorithm>

namespace Core
{
    NumericBase::NumericBase(Core::Environment *environment,
                             std::string_view name)
        : BaseType(environment, name)
    {
    }

    Type *NumericBase::higher_bit_width(Type *a, Type *b) const
    {
        if (a == nullptr || b == nullptr)
            return nullptr;

        const TypeArgument &bw = a->arguments[0], &bw_ = b->arguments[0];
        if (bw.valueless_by_exception() || bw_.valueless_by_exception())
            return nullptr;

        return std::visit(
            [&](auto &val, auto &val_) -> Type *
            {
                using Ty = std::decay_t<decltype(val)>;
                using Ty_ = std::decay_t<decltype(val_)>;

                if constexpr (std::is_same_v<Ty,
                                             std::shared_ptr<Core::Value>> &&
                              std::is_same_v<Ty_, std::shared_ptr<Core::Value>>)

                    return std::visit(
                        [&](auto &v, auto &v_) -> Type *
                        {
                            using T = std::decay_t<decltype(v)>;
                            using T_ = std::decay_t<decltype(v_)>;

                            if constexpr (std::is_same_v<T, Core::h_int> &&
                                          std::is_same_v<T_, Core::h_int>)
                            {
                                return construct_wrapper(
                                    std::max(v.u64(), v_.u64()));
                            }

                            else
                                return nullptr;
                        },
                        *val, *val_);

                else
                    return nullptr;
            },
            bw, bw_);
    }

} // namespace Core
