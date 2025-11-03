#include "core/value/ValuePool.hpp"

namespace Core
{
    ValuePool::ValuePool() { pool_.reserve(8); }

    Value *ValuePool::add(std::unique_ptr<Value> &&value)
    {
        pool_.push_back(std::move(value));
        return pool_.back().get();
    }

    Value *create_value(std::unique_ptr<Value::T> &&value,
                        InstantiatedType *type, ValueType val_type,
                        Core::PositionRange *range)
    {
        return VALUE_POOL.add(
            std::make_unique<Value>(std::move(value), type, val_type, range));
    }

    ValuePool VALUE_POOL;

} // namespace Core
