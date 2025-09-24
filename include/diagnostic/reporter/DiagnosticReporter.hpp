#pragma once

#include "diagnostic/Diagnostic.hpp"

namespace Diagnostic
{
    struct DiagnosticReporter
    {
        DiagnosticList diagnostics;

        DiagnosticReporter(DiagnosticList &&diagnostics);

        virtual std::string
        format_diagnostic(const Diagnostic &diagnostic) const = 0;

        virtual void report() const;
    };

} // namespace Diagnostic
