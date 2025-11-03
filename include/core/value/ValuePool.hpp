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

    Value *create_value(std::unique_ptr<Value::T> &&value,
                        InstantiatedType *type,
                        ValueType val_type = ValueType::RValue,
                        Core::PositionRange *range = nullptr);

    extern ValuePool VALUE_POOL;

} // namespace Core
