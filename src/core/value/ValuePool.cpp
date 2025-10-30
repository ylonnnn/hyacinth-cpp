#include "core/value/ValuePool.hpp"

namespace Core
{
    ValuePool::ValuePool() { pool_.reserve(8); }

    Value *ValuePool::add(std::unique_ptr<Value> &&value)
    {
        pool_.push_back(std::move(value));
        return pool_.back().get();
    }

    ValuePool VALUE_POOL;

} // namespace Core
