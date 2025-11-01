#pragma once

#include <optional>

#include "diagnostic/Diagnostic.hpp"
#include "lexer/Token.hpp"

namespace Diagnostic
{
    std::unique_ptr<Diagnostic>
    create_unexpected_token_error(const Lexer::Token &received,
                                  std::string expected = "");

    std::unique_ptr<Diagnostic>
    create_syntax_error(const Lexer::Token &token,
                        std::optional<Lexer::TokenType> type = std::nullopt);

} // namespace Diagnostic
