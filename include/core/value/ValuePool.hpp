#pragma once

#include <vector>

#include "core/value/Value.hpp"

namespace Core
{
    struct ValuePool
    {
        ValuePool();

        Value *add(std::unique_ptr<Value> &&value);

      private:
        std::vector<std::unique_ptr<Value>> pool_;
    };

    ReadValue *create_value(std::unique_ptr<Value::T> &&value,
                            InstantiatedType *type,
                            PositionRange *range = nullptr);

    LocatorValue *create_value(ReadValue &rvalue, PositionRange &range);

    extern ValuePool VALUE_POOL;

} // namespace Core
