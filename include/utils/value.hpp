#pragma once

#include "core/value/Value.hpp"
#include "lexer/Token.hpp"

namespace Utils
{
    Core::Value parse_val(Lexer::Token &raw_token);

} // namespace Utils
