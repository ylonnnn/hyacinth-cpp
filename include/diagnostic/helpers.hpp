#pragma once

#include "diagnostic/Diagnostic.hpp"
#include "lexer/Token.hpp"
#include <optional>

namespace Diagnostic
{
    std::unique_ptr<Diagnostic>
    create_syntax_error(const Lexer::Token &token,
                        std::optional<Lexer::TokenType> type = std::nullopt);
}
