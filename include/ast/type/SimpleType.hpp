#pragma once

#include "ast/expr/Path.hpp"
#include "ast/type/Type.hpp"

namespace AST
{
    struct SimpleType : Type
    {
        std::unique_ptr<Path> base;

        SimpleType(std::unique_ptr<Path> &&base);

        std::string to_string() const override;
        void print(std::ostream &os, uint32_t tab) const override;
    };

} // namespace AST
