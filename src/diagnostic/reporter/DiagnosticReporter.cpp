#include "diagnostic/reporter/DiagnosticReporter.hpp"

namespace Diagnostic
{
    DiagnosticReporter::DiagnosticReporter(DiagnosticList &&diagnostics)
        : diagnostics(std::move(diagnostics))
    {
    }

} // namespace Diagnostic
