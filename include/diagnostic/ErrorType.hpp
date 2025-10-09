#pragma once

#include <cstdint>

namespace Diagnostic
{
    enum class ErrorType : uint32_t
    {
        // Syntax
        UnexpectedToken = 0x190,
        InvalidLiteral,
        UnterminatedCharacterSequence,
        UnexpectedSeparator,

        MalformedLiteral,
        InvalidNumericLiteralPrefix,
        InvalidNumericLiteralDigit,

        MissingValue,
    };

}
