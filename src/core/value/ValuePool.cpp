#include "core/value/ValuePool.hpp"
#include "core/value/Value.hpp"

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

    ReadValue *create_copy(Value &value)
    {
        ReadValue &rvalue = get_rvalue(value);
        return create_value(std::make_unique<Value::T>(*rvalue.value),
                            rvalue.type, value.range);
    }

    ValuePool VALUE_POOL;

} // namespace Core
