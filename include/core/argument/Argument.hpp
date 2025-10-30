#pragma once

#include "core/value/Value.hpp"

namespace Core
{
    struct InstantiatedType;

    using GenericArgument = std::variant<InstantiatedType *, Value *>;

} // namespace Core
