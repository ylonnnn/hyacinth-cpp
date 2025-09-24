#include <iostream>

#include "diagnostic/reporter/CLIReporter.hpp"
#include "diagnostic/reporter/DiagnosticReporter.hpp"
#include "utils/style.hpp"

namespace Diagnostic
{
    namespace fs = std::filesystem;

    CLIReporter::CLIReporter(DiagnosticList &&diagnostics)
        : DiagnosticReporter(std::move(diagnostics))
    {
    }

    std::string CLIReporter::color_of(DiagnosticSeverity severity) const
    {
        return ((std::string[]){note_color_, warning_color_,
                                error_color_})[static_cast<size_t>(severity)];
    }

    std::string
    CLIReporter::severity_to_string(DiagnosticSeverity severity) const
    {
        return ((std::string[]){"note", "warning",
                                "color"})[static_cast<size_t>(severity)];
    }

    std::string CLIReporter::format_code(DiagnosticSeverity severity,
                                         uint32_t code) const
    {
        return ((std::string[]){"N0", "W0",
                                "E0"})[static_cast<size_t>(severity)] +
               std::to_string(code);
    }

    std::string
    CLIReporter::point_position_range(DiagnosticSeverity severity,
                                      const Core::PositionRange &range) const
    {
        auto &[start, end] = range;

        auto &program = start.program;
        auto &lines_ = program.source_lines;

        std::vector<std::string_view> lines;
        lines.reserve((end.row - start.row) + 1);

        lines.insert(lines.end(), lines_.begin() + (start.row - 1),
                     lines_.begin() + end.row);

        size_t tab_size = 4;
        std::string formatted, tab = utils::tab(1, tab_size),
                               color = color_of(severity);

        for (auto i = start.row - 1; i < end.row; i++)
        {
            std::string line(lines_[i]), l_no = std::to_string(i + 1);

            size_t prefix_len = (tab_size * 3) + 1 + l_no.size();
            formatted += tab + utils::Colors::Cyan + l_no + tab +
                         utils::Colors::BrightBlack + "|" +
                         utils::Styles::Reset + tab;

            // Apply color to the specific range
            auto l_start = i == start.row - 1 ? start.col - 1 : 0,
                 l_end = (i == end.row - 1 ? end.col : line.size()) - 1;

            line.insert(l_end + 1, utils::Styles::Reset);
            line.insert(l_start, color);

            formatted += line + "\n";

            formatted += utils::tab(prefix_len + l_start, 1) + color +
                         std::string((l_end - l_start) + 1, '^') + "\n";
        }

        return formatted;
    }

    std::string
    CLIReporter::format_diagnostic(const Diagnostic &diagnostic) const
    {
        auto &[severity, code, message, range, details] = diagnostic;
        auto &[row, col, offset, program] = range.start;

        fs::path current = fs::current_path(), parent = current.parent_path();

        std::string p_path = program.path.string(),
                    cwp = current.string().substr(parent.string().size()),
                    location = p_path.substr(p_path.find(cwp)) + ":" +
                               std::to_string(row) + ":" + std::to_string(col);

        std::string formatted =
            utils::Colors::BrightBlack + location + ": " + color_of(severity) +
            severity_to_string(severity) + "<" + format_code(severity, code) +
            "> " + utils::Styles::Reset + message + "\n" +
            point_position_range(severity, range);

        return formatted;
    }

    void CLIReporter::report() const
    {
        std::cout << "\n";

        for (const auto &diagnostic : diagnostics)
        {
            if (diagnostic == nullptr)
                continue;

            std::cout << format_diagnostic(*diagnostic) << "\n";
        }
    }

} // namespace Diagnostic
