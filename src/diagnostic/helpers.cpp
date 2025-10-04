#include <iostream>
#include <optional>

#include "core/position/Position.hpp"
#include "diagnostic/ErrorType.hpp"
#include "diagnostic/helpers.hpp"

namespace Diagnostic
{
    std::unique_ptr<Diagnostic>
    create_syntax_error(const Lexer::Token &token,
                        std::optional<Lexer::TokenType> type)
    {
        return std::make_unique<Diagnostic>(
            DiagnosticSeverity::Error,
            static_cast<uint32_t>(ErrorType::UnexpectedToken),
            Core::PositionRange(token.range),
            "unexpected \"" + std::string(token.value) +
                (type ? std::string("\", expected \"") +
                            Lexer::type_to_string(*type)
                      : "") +
                "\".");
    }

} // namespace Diagnostic
