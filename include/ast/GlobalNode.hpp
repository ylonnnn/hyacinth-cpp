#pragma once

#include "ast/Node.hpp"

namespace AST
{
    class GlobalNode : virtual public Node
    {
      public:
        GlobalNode() = default;
        virtual ~GlobalNode() = default;

        virtual void print(std::ostream &os, uint8_t tab) const override = 0;
    };

} // namespace AST
