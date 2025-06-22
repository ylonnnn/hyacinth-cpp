#include <iostream>
#include <unordered_map>

#include "diagnostic/ErrorDiagnostic.hpp"
#include "utils/control.hpp"
#include "utils/style.hpp"

namespace Diagnostic
{
    std::unordered_map<ErrorType, const char *> ERROR_CODES;

    void initialize_codes()
    {
        ERROR_CODES.reserve(32);

        ERROR_CODES[ErrorTypes::General::Syntax] = "HYC::SYNTAX";
        ERROR_CODES[ErrorTypes::Type::Mismatch] = "HYC::TYPE_MISMATCH";
        ERROR_CODES[ErrorTypes::Type::UnknownType] = "HYC::TYPE_UNKNOWN";
        ERROR_CODES[ErrorTypes::Type::InvalidArgumentType] =
            "HYC::INVALID_ARGUMENT_TYPE";
        ERROR_CODES[ErrorTypes::Type::InvalidReturnType] =
            "HYC::INVALID_RETURN_TYPE";
        ERROR_CODES[ErrorTypes::Modification::ReadValueModification] =
            "HYC::RVALUE_MODIFICATION";
        ERROR_CODES[ErrorTypes::Modification::ImmutableValueModification] =
            "HYC::IMMUTABLE_MODIFICATION";
        ERROR_CODES[ErrorTypes::Uninitialization::UninitializedValue] =
            "HYC::UNINITIALIZED_VALUE";
        ERROR_CODES[ErrorTypes::Uninitialization::UninitializedImmutable] =
            "HYC::UNINITIALIZED_IMMUTABLE";
    }

    bool initialized_codes = (initialize_codes(), true);

    Utils::TextStyle ERROR_GENERAL = Utils::Colors::Red,
                     ERROR_EMPHASIS = Utils::Colors::BrightRed;

    ErrorDiagnostic::ErrorDiagnostic(std::unique_ptr<AST::Node> node,
                                     ErrorType error_type,
                                     std::string &&message,
                                     std::string &&emphasis_message)
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
        const Program::Position &position = node_->position();
        auto &[row, col, program] = node_->position();

        std::cout << "\n\n";

        std::cout << ERROR_GENERAL << "Error <"
                  << error_type_to_string(error_type_) << "> "
                  << Utils::Styles::Reset << message_ << "\n\n";

        emphasize_position((DiagnosticEmphasis){
            .message = emphasis_message_,
            .position = const_cast<Program::Position &>(position),
            .length = node_->end_pos(),
            .emphasis = ERROR_EMPHASIS,
            .trace = ERROR_GENERAL,
            .pointer = ERROR_GENERAL,
        });

        for (std::unique_ptr<Diagnostic> &detail : details)
        {
            detail->report();
        }
    }

} // namespace Diagnostic
