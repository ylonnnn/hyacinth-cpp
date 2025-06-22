#pragma once

#include <functional>

#include "lexer/Token.hpp"
#include "parser/Parser.hpp"

namespace Parser
{
    using NudHandler = std::function<void(Parser &parser)>;
    using NudHandlerMap = std::unordered_map<Lexer::TokenType, NudHandler>;

    extern NudHandlerMap NUD_HANDLER_MAP;

} // namespace Parser
