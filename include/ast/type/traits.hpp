#pragma once

#include <type_traits>

#include "ast/type/ConstantType.hpp"

namespace AST
{
    template <typename T> struct is_constant_literal_type : std::false_type
    {
    };

    template <> struct is_constant_literal_type<ConstantType> : std::true_type
    {
    };

} // namespace AST
