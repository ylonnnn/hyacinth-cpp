#pragma once

#include "ast/Node.hpp"

namespace AST
{
    struct GlobalNode : virtual Node
    {
        GlobalNode() = default;
        virtual ~GlobalNode() = default;

        virtual void print(std::ostream &os, uint8_t tab) const override = 0;
    };

} // namespace AST
