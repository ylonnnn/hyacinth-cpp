#include "parser/LeftDenotation.hpp"
#include "lexer/Token.hpp"
#include "parser/BindingPower.hpp"

namespace Parser
{
    LedHandlerMap LED_HANDLER_MAP;

    void initialize_led_handler(Lexer::TokenType type, BindingPowerPair bp_pair,
                                LedHandler handler)
    {
        initialize_binding_power(type, bp_pair);
        LED_HANDLER_MAP[type] = handler;
    }

} // namespace Parser
