#include "diagnostic/WarningDiagnostic.hpp"
#include "utils/control.hpp"

namespace Diagnostic
{
    std::unordered_map<WarningType, const char *> WARN_CODES;

    static void initialize_codes()
    {
        WARN_CODES.reserve(32);

        WARN_CODES[WarningType::Unreachable] = "HYC::UNREACHABLE";
        WARN_CODES[WarningType::Unused] = "HYC::UNUSED";
    }

    static bool initialized_codes = (initialize_codes(), true);

    Utils::TextStyle WARNING_GENERAL = Utils::Colors::Yellow,
                     WARNING_EMPHASIS = Utils::Colors::BrightYellow;

    WarningDiagnostic::WarningDiagnostic(std::unique_ptr<AST::Node> node,
                                         WarningType warn_type,
                                         std::string &&message,
                                         std::string &&emphasis_message)
        : Diagnostic(std::move(node), std::move(message),
                     std::move(emphasis_message)),
          warn_type_(warn_type)
    {
    }

    WarningType WarningDiagnostic::warn_type() { return warn_type_; }

    const char *WarningDiagnostic::warn_type_to_string(WarningType type)
    {
        auto it = WARN_CODES.find(type);
        if (it == WARN_CODES.end())
            Utils::terminate("Unknown error type provided!", EXIT_FAILURE);

        return it->second;
    }

    void WarningDiagnostic::report()
    {
        const Program::Position &position = node_->position();

        std::cout << "\n\n";

        std::cout << WARNING_GENERAL << "Warning <"
                  << warn_type_to_string(warn_type_) << "> "
                  << Utils::Styles::Reset << message_ << "\n\n";

        emphasize_position((DiagnosticEmphasis){
            .message = emphasis_message_,
            .position = const_cast<Program::Position &>(position),
            .length = node_->end_pos(),
            .emphasis = WARNING_EMPHASIS,
            .trace = WARNING_GENERAL,
            .pointer = WARNING_GENERAL,
        });

        for (std::unique_ptr<Diagnostic> &detail : details)
        {
            detail->report();
        }
    }

} // namespace Diagnostic
