#include "core/result/Result.hpp"

namespace Core
{
    std::ostream &operator<<(std::ostream &os, ResultStatus status)
    {
        return os << (status == ResultStatus::Success ? "Success" : "Fail");
    }

} // namespace Core
