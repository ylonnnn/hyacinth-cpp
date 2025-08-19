#include <string>

#include "diagnostic/Diagnostic.hpp"
#include "utils/control.hpp"
#include "utils/style.hpp"

namespace Diagnostic
{
    static std::vector<std::string_view>
    get_lines(Core::ProgramFile &program, std::pair<size_t, size_t> line_range)
    {
        const auto &[start, end] = line_range;

        std::vector<std::string_view> lines_, &lines = program.lines();
        size_t line_count = lines.size();

        if (start > line_count || end > line_count)
        {
            Utils::terminate("Invalid line range provided!", EXIT_FAILURE);
            return lines_;
        }

        lines_.insert(lines_.end(), lines.begin() + (start - 1),
                      lines.end() + (end - 1));

        return lines_;
    }

    Diagnostic::Diagnostic(AST::Node *node, const std::string &message,
                           const std::string &submessage)
        : node_(node), message_(message), submessage_(submessage)
    {
    }

    AST::Node &Diagnostic::node() { return *node_; }

    void Diagnostic::add_detail(std::unique_ptr<Diagnostic> detail)
    {
        details_.push_back(std::move(detail));
    }

    void Diagnostic::construct_emphasis(DiagnosticEmphasis options)
    {
        auto &[message, position, end_position, emphasis, trace, pointer] =
            options;
        auto &[row, col, program] = position;

        std::vector<std::string_view> lines = get_lines(
            program, std::pair<size_t, size_t>{position.row, end_position.row});

        constexpr size_t tab_size = 3;
        size_t start = row - 1, end = end_position.row;

        std::string offset;

        for (size_t i = start; i < end; i++)
        {
            size_t idx = i - start;
            std::string_view line = lines[idx];

            auto _start = i == start, _end = i == end - 1;
            size_t line_start =
                       _start ? position.col : line.find_first_not_of(' ') + 1,
                   line_end = _end ? end_position.col : line.size();

            std::string tab = Utils::tab(tab_size, 1),
                        row = std::to_string(i + 1),
                        prefix = tab + row + "  | " + tab;

            size_t prefix_len = prefix.size();
            auto display = (prefix + std::string(line) + "\n");

            display.insert(prefix_len + line_end, Utils::Styles::Reset);
            display.insert(prefix_len + line_start - 1, emphasis);

            display.insert(tab_size + row.size(), Utils::Styles::Reset);
            display.insert(tab_size, Utils::Colors::Yellow);

            constructed_ += display;

            // Pointer
            offset = Utils::tab((prefix.size() + line_start) - 1, 1);
            constructed_ += (offset + pointer + "^" +
                             std::string(line_end - line_start, '^') +
                             Utils::Styles::Reset + "\n");
        }

        if (!message.empty())
            constructed_ += (offset + pointer + "| " + std::string(message) +
                             Utils::Styles::Reset + "\n");

        constructed_ +=
            (std::string("\n\t\t") + trace + "at " + program.path().string() +
             ":" + std::to_string(row) + ":" + std::to_string(col) +
             Utils::Styles::Reset + "\n");
    }

    void Diagnostic::report() const
    {
        std::cout << constructed_;

        for (auto &detail : details_)
        {
            if (detail == nullptr)
                continue;

            detail->report();
        }
    }

} // namespace Diagnostic
