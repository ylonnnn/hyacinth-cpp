#include <string>

#include "diagnostic/Diagnostic.hpp"

namespace Diagnostic
{
    Diagnostic::Diagnostic(DiagnosticSeverity severity, uint32_t code,
                           Core::PositionRange &&range, std::string &&message)
        : severity(severity), code(code), range(std::move(range)),
          message(std::move(message))
    {
    }

    Diagnostic &Diagnostic::add_detail(std::unique_ptr<Diagnostic> &&detail)
    {
        details.push_back(std::move(detail));
        return *this;
    }

} // namespace Diagnostic
