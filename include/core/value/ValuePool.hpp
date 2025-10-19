#pragma once

#include <vector>

#include "core/value/Value.hpp"

namespace Core
{
    struct ValuePool
    {
        ValuePool();

        Value *add(Value &&value);

      private:
        std::vector<Value> pool_;
    };

    extern ValuePool VALUE_POOL;

} // namespace Core
