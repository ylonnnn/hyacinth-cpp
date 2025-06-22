#include "parser/NullDenotation.hpp"
#include "lexer/Token.hpp"
#include "parser/BindingPower.hpp"

namespace Parser
{
    NudHandlerMap NUD_HANDLER_MAP;

    void initialize_nud_handler(Lexer::TokenType type, BindingPowerPair bp_pair,
                                NudHandler handler)
    {
        initialize_binding_power(type, bp_pair);
        NUD_HANDLER_MAP[type] = handler;
    }

} // namespace Parser
