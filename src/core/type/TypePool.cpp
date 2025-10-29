#include "core/type/Type.hpp"

namespace Core
{
    std::string &TypePool::type_to_string(InstantiatedType &type) const
    {
        return type.to_string();
    }

    TypePool TYPE_POOL;

} // namespace Core
