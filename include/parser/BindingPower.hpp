#pragma once

#include <unordered_map>
#include <utility>

#include "lexer/Token.hpp"

namespace Parser
{
    enum class BindingPower
    {
        Default = 0,
        Comma,
        Assignment,
        Logical,
        Relational,
        Additive,
        Multiplicative,
        Unary,
        Function_call,
        Member_access,
        Primary,
    };

    using BindingPowerPair = std::pair<float, float>;
    using BindingPowerMap =
        std::unordered_map<Lexer::TokenType, BindingPowerPair>;

    extern BindingPowerMap BINDING_POWER_MAP;

    void initialize_binding_power(Lexer::TokenType type,
                                  BindingPowerPair bp_pair);

} // namespace Parser
