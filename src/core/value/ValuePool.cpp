#include "core/value/ValuePool.hpp"

namespace Core
{
    ValuePool::ValuePool() { pool_.reserve(8); }

    Value *ValuePool::add(std::unique_ptr<Value> &&value)
    {
        pool_.push_back(std::move(value));
        return pool_.back().get();
    }

    ReadValue *create_value(std::unique_ptr<Value::T> &&value,
                            InstantiatedType *type, PositionRange *range)
    {
        return static_cast<ReadValue *>(VALUE_POOL.add(
            std::make_unique<ReadValue>(std::move(value), type, range)));
    }

    LocatorValue *create_value(ReadValue &rvalue, PositionRange &range)
    {
        return static_cast<LocatorValue *>(
            VALUE_POOL.add(std::make_unique<LocatorValue>(rvalue, range)));
    }

    ValuePool VALUE_POOL;

} // namespace Core
