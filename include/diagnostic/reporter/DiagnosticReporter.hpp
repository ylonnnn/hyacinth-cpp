#pragma once

#include <array>

#include "diagnostic/Diagnostic.hpp"

namespace Diagnostic
{
    using DiagnosticReportStatusResult = std::array<uint32_t, 3>;

    struct DiagnosticReporter
    {
        DiagnosticList diagnostics;

        DiagnosticReporter(DiagnosticList &&diagnostics);

        virtual std::string
        format_diagnostic(const Diagnostic &diagnostic) const = 0;

        virtual DiagnosticReportStatusResult report() const = 0;
    };

} // namespace Diagnostic
