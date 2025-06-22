#pragma once

#include <functional>

#include "ast/expr/Expr.hpp"
#include "lexer/Token.hpp"
#include "parser/Parser.hpp"

namespace Parser
{
    using LedHandler =
        std::function<void(Parser &parser, AST::Expr &expr, float right_bp)>;
    using LedHandlerMap = std::unordered_map<Lexer::TokenType, LedHandler>;

    extern LedHandlerMap LED_HANDLER_MAP;

} // namespace Parser
