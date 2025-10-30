#pragma once

#include "core/value/Value.hpp"
#include "lexer/Token.hpp"

namespace utils
{
    Core::Value *parse_val(Lexer::Token &raw_token);

} // namespace utils
