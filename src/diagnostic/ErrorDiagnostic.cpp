#include <iostream>
#include <unordered_map>

#include "ast/expr/LiteralExpr.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "utils/control.hpp"
#include "utils/style.hpp"

namespace Diagnostic
{
    std::unordered_map<ErrorType, const char *> ERROR_CODES;

    static void initialize_codes()
    {
        ERROR_CODES.reserve(32);

        ERROR_CODES[ErrorTypes::Syntax::UnexpectedToken] =
            "HYC::SYNTAX::UNEXPECTED_TOKEN";
        ERROR_CODES[ErrorTypes::Syntax::MissingIdentifier] =
            "HYC::SYNTAX:MISSING_IDENTIFIER";
        ERROR_CODES[ErrorTypes::Syntax::MissingValue] =
            "HYC::SYNTAX:MISSING_VALUE";
        ERROR_CODES[ErrorTypes::Syntax::MissingKeyword] =
            "HYC::SYNTAX:MISSING_KEYWORD";
        ERROR_CODES[ErrorTypes::Syntax::MissingOperator] =
            "HYC::SYNTAX:MISSING_OPERATOR";
        ERROR_CODES[ErrorTypes::Syntax::MissingType] =
            "HYC::SYNTAX:MISSING_TYPE";
        ERROR_CODES[ErrorTypes::Syntax::MissingReturnType] =
            "HYC::SYNTAX:MISSING_RETURN_TYPE";
        ERROR_CODES[ErrorTypes::Syntax::MissingMutabilityModifier] =
            "HYC::SYNTAX:MISSING_MUTABILITY_MODIFIER";

        ERROR_CODES[ErrorTypes::Type::Mismatch] = "HYC::TYPE:MISMATCH";
        ERROR_CODES[ErrorTypes::Type::UnknownType] = "HYC::TYPE:UNKNOWN";
        ERROR_CODES[ErrorTypes::Type::InvalidArgumentType] =
            "HYC::TYPE:INVALID_ARGUMENT_TYPE";
        ERROR_CODES[ErrorTypes::Type::InvalidReturnType] =
            "HYC::TYPE:INVALID_RETURN_TYPE";

        ERROR_CODES[ErrorTypes::Modification::ReadValueModification] =
            "HYC::MODIFICATION:RVALUE_MODIFICATION";
        ERROR_CODES[ErrorTypes::Modification::ImmutableValueModification] =
            "HYC::MODIFICATION:IMMUTABLE_MODIFICATION";

        ERROR_CODES[ErrorTypes::Uninitialization::UninitializedValue] =
            "HYC::UNINITIALIZATION:UNINITIALIZED_VALUE";
        ERROR_CODES[ErrorTypes::Uninitialization::UninitializedImmutable] =
            "HYC::UNINITIALIZATION:UNINITIALIZED_IMMUTABLE";
    }

    static bool initialized_codes = (initialize_codes(), true);

    ErrorDiagnostic::ErrorDiagnostic(std::unique_ptr<AST::Node> node,
                                     ErrorType error_type, std::string message,
                                     std::string emphasis_message)
        : Diagnostic(std::move(node), std::move(message),
                     std::move(emphasis_message)),
          error_type_(error_type)
    {
    }

    ErrorType ErrorDiagnostic::error_type() { return error_type_; }

    const char *ErrorDiagnostic::error_type_to_string(ErrorType type)
    {
        auto it = ERROR_CODES.find(type);
        if (it == ERROR_CODES.end())
            Utils::terminate("Unknown error type provided!", EXIT_FAILURE);

        return it->second;
    }

    void ErrorDiagnostic::report()
    {
        const ::Program::Position &position = node_->position();

        std::cout << "\n\n";

        std::cout << ERR_GEN << "Error <" << error_type_to_string(error_type_)
                  << "> " << Utils::Styles::Reset << message_ << "\n\n";

        emphasize_position((DiagnosticEmphasis){
            .message = emphasis_message_,
            .position = const_cast<::Program::Position &>(position),
            .length = node_->end_pos(),
            .emphasis = ERR_EMPH,
            .trace = ERR_GEN,
            .pointer = ERR_GEN,
        });

        for (std::unique_ptr<Diagnostic> &detail : details)
        {
            detail->report();
        }
    }

    std::unique_ptr<ErrorDiagnostic>
    create_syntax_error(Lexer::Token *token,
                        std::optional<Lexer::TokenType> expected)
    {
        if (token == nullptr)
            Utils::terminate("Token cannot be a nullptr!", EXIT_FAILURE);

        bool expects = !!expected;

        return std::make_unique<ErrorDiagnostic>(
            std::make_unique<AST::LiteralExpr>(*token),
            ErrorTypes::Syntax::UnexpectedToken,
            std::string("Unexpected \"") + ERR_EMPH +
                std::string(token->value) + Utils::Styles::Reset + "\"." +
                (expects ? std::string(" Expected \"") + ERR_EMPH +
                               Lexer::type_to_string(*expected) +
                               Utils::Styles::Reset + "."
                         : ""),
            ERR_GEN + std::string("Received ") +
                Lexer::type_to_string(token->type) +
                (expects ? " instead" : "") + ".");
    }

} // namespace Diagnostic
