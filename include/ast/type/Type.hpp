#pragma once

#include <cstdint>

#include "ast/Node.hpp"

namespace AST
{
    struct Type : virtual Node
    {
        Type() = default;
        virtual ~Type() = default;

        virtual std::string to_string() const = 0;
        virtual void print(std::ostream &os, uint8_t tab) const = 0;
    };

} // namespace AST
