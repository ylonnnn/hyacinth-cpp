#include "core/value/ValuePool.hpp"

namespace Core
{
    ValuePool::ValuePool() { pool_.reserve(8); }

    Value *ValuePool::add(Value &&value)
    {
        pool_.push_back(std::move(value));

        return &pool_.back();
    }

    ValuePool VALUE_POOL;

} // namespace Core
