#include <optional>

#include "core/position/Position.hpp"
#include "diagnostic/ErrorType.hpp"
#include "diagnostic/helpers.hpp"

namespace Diagnostic
{
    std::unique_ptr<Diagnostic>
    create_unexpected_token_error(const Lexer::Token &received,
                                  std::string expected)
    {
        return std::make_unique<Diagnostic>(
            DiagnosticSeverity::Error,
            static_cast<uint32_t>(ErrorType::UnexpectedToken), received.range,
            "unexpected \"" + std::string(received.value) +
                (expected.size() ? std::string("\", expected \"") + expected
                                 : "") +
                "\".");
    }

    std::unique_ptr<Diagnostic>
    create_syntax_error(const Lexer::Token &token,
                        std::optional<Lexer::TokenType> type)
    {
        return create_unexpected_token_error(
            token, type ? Lexer::type_to_string(*type) : "");
    }

} // namespace Diagnostic
