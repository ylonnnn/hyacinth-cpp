#include "core/type/builtin/numeric/BitWidth.hpp"
#include "utils/dev.hpp"

namespace Core
{
    BitWidthType::BitWidthType(Environment &environment)
        : BaseType(environment, "bw"), min_(1), max_(64)
    {
    }

    TypeResult
    BitWidthType::assignable(const std::vector<GenericArgument> &arguments,
                             Value *value) const
    {
        TypeResult result{ResultStatus::Success, nullptr, {}};

        auto ptr = std::get_if<integer>(value->value.get());
        if (value == nullptr || ptr == nullptr)
        {
            utils::todo("throw error: expected value of type bit width, "
                        "received 'type'");
            return result;
        }

        integer &i_val = *ptr;
        auto val = i_val.as<uint64_t>();

        if (val < min_ || val > max_)
        {
            utils::todo("throw error: expected value of type bit width, "
                        "received 'type'");
            utils::todo("suggest: values within {min} to {max} are the "
                        "expected values.");

            return result;
        }

        return result;
    }

} // namespace Core
