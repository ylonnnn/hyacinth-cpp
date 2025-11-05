#include <string>

#include "diagnostic/Diagnostic.hpp"

namespace Diagnostic
{
    Diagnostic::Diagnostic(DiagnosticSeverity severity, uint32_t code,
                           Diagnostic::Diagnostic::PosRange range,
                           std::string &&message)
        : severity(severity), code(code), message(std::move(message)),
          range_(std::move(range))
    {
        details.reserve(4);
    }

    Core::PositionRange &Diagnostic::range()
    {
        return std::visit(
            [](auto &val) -> Core::PositionRange &
            {
                using T = std::decay_t<decltype(val)>;

                if constexpr (std::is_same_v<T, Core::PositionRange *>)
                    return *val;
                else
                    return val;
            },
            range_);
    }

    const Core::PositionRange &Diagnostic::range() const
    {
        return const_cast<Diagnostic *>(this)->range();
    }

    Diagnostic &Diagnostic::add_detail(std::unique_ptr<Diagnostic> &&detail)
    {
        details.push_back(std::move(detail));
        return *this;
    }

    Diagnostic &Diagnostic::add_detail(DiagnosticSeverity severity,
                                       uint32_t code,
                                       Diagnostic::Diagnostic::PosRange range,
                                       std::string &&message)
    {
        details.push_back(std::make_unique<Diagnostic>(
            severity, code, std::move(range), std::move(message)));

        return *this;
    }

} // namespace Diagnostic
