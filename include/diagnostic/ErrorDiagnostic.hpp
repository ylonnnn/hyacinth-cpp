#pragma once

#include <optional>
#include <unordered_map>
#include <variant>

#include "ast/Node.hpp"
#include "ast/type/Type.hpp"
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
            MissingImportTarget,
            MissingIdentifier,
            MissingValue,
            MissingKeyword,
            MissingOperator,
            MissingType,
            MissingReturnType,
            MissingMutabilityModifier,
            InvalidFieldSpecifier,
            FieldDuplcation,
        };

        enum class Type
        {
            Mismatch = 1,
            UnrecognizedType,
            InvalidVariableType,
            InvalidArgumentType,
            InvalidTypeArgumentType,
            InvalidReturnType,
            InvalidInstantiationType,
        };

        enum class Semantic
        {
            NonCallableInvocation = 1,
            UnrecognizedSymbol,
            InvalidArgumentCount,
            Duplication,
            InvalidReturnUsage,
            IllegalShadowing,
            NoViableOperatorOverload,
            UnrecognizedField,
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
                     ErrorTypes::Semantic, ErrorTypes::Modification,
                     ErrorTypes::Uninitialization>;

    extern std::unordered_map<ErrorType, const char *> ERROR_CODES;

    class ErrorDiagnostic : public Diagnostic
    {
      private:
        ErrorType error_type_;

      public:
        ErrorDiagnostic(AST::Node *node, ErrorType error_type,
                        const std::string &message,
                        const std::string &submessage);

        ErrorType error_type();

        const char *error_type_to_string(ErrorType type);

        void construct() override;
    };

    std::unique_ptr<ErrorDiagnostic> create_syntax_error(
        Lexer::Token *token,
        std::optional<Lexer::TokenType> expected = std::nullopt);

    std::unique_ptr<ErrorDiagnostic> create_unknown_type_error(AST::Type *type);

} // namespace Diagnostic
