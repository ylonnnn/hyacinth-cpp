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

        void report() const override;

      private:
        utils::TextStyle note_color_ = utils::Colors::Blue,
                         warning_color_ = utils::Colors::Yellow,
                         error_color_ = utils::Colors::Red;
    };

} // namespace Diagnostic
