#include "parser/BindingPower.hpp"
#include "lexer/Token.hpp"

namespace Parser
{
    BindingPowerMap BINDING_POWER_MAP;

    void initialize_binding_power(Lexer::TokenType type,
                                  BindingPowerPair bp_pair)
    {
        BINDING_POWER_MAP[type] = bp_pair;
    }

} // namespace Parser
