#pragma once

#include "diagnostic/reporter/DiagnosticReporter.hpp"
#include "utils/style.hpp"

namespace Diagnostic
{
    struct CLIReporter : DiagnosticReporter
    {
        CLIReporter(DiagnosticList &&diagnostics);

        std::string color_of(DiagnosticSeverity severity) const;
        std::string severity_to_string(DiagnosticSeverity severity) const;
        std::string format_code(DiagnosticSeverity severity,
                                uint32_t code) const;

        std::string
        point_position_range(DiagnosticSeverity severity,
                             const Core::PositionRange &range) const;

        std::string
        format_diagnostic(const Diagnostic &diagnostic) const override;

        DiagnosticReportStatusResult report() const override;

        utils::TextStyle note_color = utils::Colors::BrightBlue,
                         warning_color = utils::Colors::Yellow,
                         error_color = utils::Colors::Red;
    };

} // namespace Diagnostic
