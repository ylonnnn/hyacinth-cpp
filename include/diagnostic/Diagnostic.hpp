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
        using PosRange =
            std::variant<Core::PositionRange, Core::PositionRange *>;

        DiagnosticSeverity severity;
        uint32_t code;
        std::string message;
        DiagnosticList details;

        PosRange range_;

        Diagnostic(DiagnosticSeverity severity, uint32_t code, PosRange range,
                   std::string &&message);

        Core::PositionRange &range();
        const Core::PositionRange &range() const;

        Diagnostic &add_detail(std::unique_ptr<Diagnostic> &&detail);
        Diagnostic &add_detail(DiagnosticSeverity severity, uint32_t code,
                               PosRange range, std::string &&message);
    };

    using DiagnosticList = Diagnostic::DiagnosticList;

} // namespace Diagnostic
