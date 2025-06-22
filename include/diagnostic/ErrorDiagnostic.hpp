#pragma once

#include <unordered_map>
#include <variant>

#include "diagnostic/Diagnostic.hpp"

namespace Diagnostic
{
    namespace ErrorTypes
    {
        enum class General
        {
            Syntax = 1,
        };

        enum class Type
        {
            Mismatch = 1,
            UnknownType,
            InvalidArgumentType,
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
        std::variant<ErrorTypes::General, ErrorTypes::Type,
                     ErrorTypes::Modification, ErrorTypes::Uninitialization>;

    extern std::unordered_map<ErrorType, const char *> ERROR_CODES;

    class ErrorDiagnostic : public Diagnostic
    {
      private:
        ErrorType error_type_;

      public:
        ErrorDiagnostic(std::unique_ptr<AST::Node> node, ErrorType error_type,
                        std::string &&message, std::string &&emphasis_message);

        ErrorType error_type();

        const char *error_type_to_string(ErrorType type);
        void report() override;
    };

} // namespace Diagnostic
