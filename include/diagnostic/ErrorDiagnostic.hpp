#pragma once

#include <optional>
#include <unordered_map>
#include <variant>

#include "diagnostic/Diagnostic.hpp"
#include "lexer/Token.hpp"

namespace Diagnostic
{
    constexpr auto ERR_GEN = Utils::Colors::Red,
                   ERR_EMPH = Utils::Colors::BrightRed;

    namespace ErrorTypes
    {
        enum class General
        {
        };

        enum class Syntax
        {
            UnexpectedToken = 1,
            MissingIdentifier,
            MissingValue,
            MissingKeyword,
            MissingOperator,
            MissingType,
            MissingReturnType,
            MissingMutabilityModifier,
        };

        enum class Type
        {
            Mismatch = 1,
            UnknownType,
            InvalidArgumentType,
            InvalidTypeArgumentType,
            InvalidReturnType,
        };

        enum class Modification
        {
            ReadValueModification = 1,
            ImmutableValueModification,
        };

        enum class Uninitialization
        {
            UninitializedValue = 1,
            UninitializedImmutable,
        };
    } // namespace ErrorTypes

    using ErrorType =
        std::variant<ErrorTypes::General, ErrorTypes::Syntax, ErrorTypes::Type,
                     ErrorTypes::Modification, ErrorTypes::Uninitialization>;

    extern std::unordered_map<ErrorType, const char *> ERROR_CODES;

    class ErrorDiagnostic : public Diagnostic
    {
      private:
        ErrorType error_type_;

      public:
        ErrorDiagnostic(AST::Node *node, ErrorType error_type,
                        const std::string & message, const std::string & submessage);

        ErrorType error_type();

        const char *error_type_to_string(ErrorType type);

        void construct() override;
    };

    std::unique_ptr<ErrorDiagnostic> create_syntax_error(
        Lexer::Token *token,
        std::optional<Lexer::TokenType> expected = std::nullopt);

} // namespace Diagnostic
