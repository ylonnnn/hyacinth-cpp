#pragma once

#include "core/type/Type.hpp"

namespace Core::TypePool
{
    InstantiatedType *add(std::unique_ptr<InstantiatedType> &&type);

} // namespace Core::TypePool
