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

    WarningDiagnostic::WarningDiagnostic(AST::Node *node, WarningType warn_type,
                                         const std::string &message,
                                         const std::string &submessage)
        : Diagnostic(node, message, submessage), warn_type_(warn_type)
    {
        construct();
    }

    WarningType WarningDiagnostic::warn_type() { return warn_type_; }

    const char *WarningDiagnostic::warn_type_to_string(WarningType type)
    {
        auto it = WARN_CODES.find(type);
        if (it == WARN_CODES.end())
            Utils::terminate("Unknown error type provided!", EXIT_FAILURE);

        return it->second;
    }

    void WarningDiagnostic::construct()
    {
        const Core::Position &position = node_->position();

        constructed_ += std::string("\n\n") + WARN_GEN + "Warning <" +
                        warn_type_to_string(warn_type_) + "> " +
                        Utils::Styles::Reset + message_ + "\n\n";

        construct_emphasis((DiagnosticEmphasis){
            .message = submessage_,
            .position = const_cast<Core::Position &>(position),
            .length = node_->end_pos(),
            .emphasis = WARN_EMPH,
            .trace = WARN_GEN,
            .pointer = WARN_GEN,
        });
    }

} // namespace Diagnostic
