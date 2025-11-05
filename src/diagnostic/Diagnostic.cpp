#include <string>

#include "diagnostic/Diagnostic.hpp"

namespace Diagnostic
{
    Diagnostic::Diagnostic(DiagnosticSeverity severity, uint32_t code,
                           const Core::PositionRange &range,
                           std::string &&message)
        : severity(severity), code(code), message(std::move(message)),
          range(range)
    {
        details.reserve(4);
    }

    Diagnostic &Diagnostic::add_detail(std::unique_ptr<Diagnostic> &&detail)
    {
        details.push_back(std::move(detail));
        return *this;
    }

    Diagnostic &Diagnostic::add_detail(DiagnosticSeverity severity,
                                       uint32_t code,
                                       const Core::PositionRange &range,
                                       std::string &&message)
    {
        details.push_back(std::make_unique<Diagnostic>(severity, code, range,
                                                       std::move(message)));

        return *this;
    }

} // namespace Diagnostic
