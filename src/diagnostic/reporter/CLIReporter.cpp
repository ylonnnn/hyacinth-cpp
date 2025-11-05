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
        return ((std::string[]){note_color, warning_color,
                                error_color})[static_cast<size_t>(severity)];
    }

    std::string
    CLIReporter::severity_to_string(DiagnosticSeverity severity) const
    {
        return ((std::string[]){"note", "warning",
                                "error"})[static_cast<size_t>(severity)];
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
                                      const Core::PositionRange &range,
                                      const std::string &prefix) const
    {
        auto &start = range.start(), &end = range.end();

        auto &program = start.program.get();
        auto &lines_ = program.source_lines;

        std::vector<std::string_view> lines;

        lines.reserve((end.row - start.row) + 1);
        lines.insert(lines.end(), lines_.begin() + (start.row - 1),
                     lines_.begin() + end.row);

        size_t tab_size = 4, l_size = lines.size();
        std::string formatted, tab = utils::tab(1, tab_size),
                               color = color_of(severity);
        for (size_t i = 0; i < l_size; ++i)
        {
            std::string line(lines[i]), l_no = std::to_string(start.row + i);

            size_t prefix_len = (tab_size * 3) + 1 + l_no.size();
            formatted += prefix + tab + utils::Colors::Cyan + l_no + tab +
                         utils::Colors::BrightBlack + "|" +
                         utils::Styles::Reset + tab;

            // Apply color to the specific range
            auto l_start = i == 0 ? start.col - 1 : 0,
                 l_end = i == l_size - 1 ? end.col - 1
                                         : line.size() - (line.size() != 0);

            if (line.size())
            {
                size_t e_pos = l_end + 1;

                line.insert(e_pos > line.size() ? line.size() : e_pos,
                            utils::Styles::Reset);
                line.insert(l_start, color);
            }

            formatted += line + "\n" + prefix +
                         utils::tab(prefix_len + l_start, 1) + color +
                         std::string((l_end - l_start) + 1, '^') + "\n";
        }

        return formatted;
    }

    std::string CLIReporter::format_diagnostic(const Diagnostic &diagnostic,
                                               uint32_t indentation) const
    {
        auto &[severity, code, message, details, _] = diagnostic;
        auto &range = diagnostic.range();
        auto &[row, col, offset, program] = range.start();

        fs::path current = fs::current_path(), parent = current.parent_path();

        std::string p_path = program.get().path.string(),
                    cwp = current.string().substr(parent.string().size()),
                    location = p_path.substr(p_path.find(cwp)) + ":" +
                               std::to_string(row) + ":" + std::to_string(col),
                    indent_str = std::string(indentation * 4, ' ');

        std::string formatted =
            utils::Colors::BrightBlack + indent_str + location + ": " +
            color_of(severity) + severity_to_string(severity) + "<" +
            format_code(severity, code) + "> " + utils::Styles::Reset +
            message + "\n" + point_position_range(severity, range, indent_str);

        for (const auto &detail : diagnostic.details)
        {
            if (detail == nullptr) [[unlikely]]
                continue;

            std::string indent_str((indentation + 1) * 4, ' '), pnl = "|\n";

            formatted += utils::Colors::White + indent_str +
                         format_diagnostic(*detail, indentation + 1);
        }

        return formatted;
    }

    DiagnosticReportStatusResult CLIReporter::report() const
    {
        DiagnosticReportStatusResult result = {0, 0, 0};

        std::cout << "\n";

        for (const auto &diagnostic : diagnostics)
        {
            if (diagnostic == nullptr)
                continue;

            ++result[static_cast<size_t>(diagnostic->severity)];

            std::cout << format_diagnostic(*diagnostic) << "\n";
        }

        return result;
    }

} // namespace Diagnostic
