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

    extern ValuePool VALUE_POOL;

} // namespace Core
