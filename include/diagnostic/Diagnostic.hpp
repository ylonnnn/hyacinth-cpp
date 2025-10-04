#pragma once

#include "core/position/Position.hpp"

namespace Diagnostic
{
    enum class DiagnosticSeverity
    {
        Note,
        Warning,
        Error,
    };

    struct Diagnostic
    {
        using DiagnosticList = std::vector<std::unique_ptr<Diagnostic>>;

        DiagnosticSeverity severity;
        uint32_t code;
        std::string message;
        Core::PositionRange range;
        DiagnosticList details;

        Diagnostic(DiagnosticSeverity severity, uint32_t code,
                   Core::PositionRange &&range, std::string &&message);

        Diagnostic &add_detail(std::unique_ptr<Diagnostic> &&detail);
    };

    using DiagnosticList = Diagnostic::DiagnosticList;

} // namespace Diagnostic
