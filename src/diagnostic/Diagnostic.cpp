#include <sstream>
#include <string>

#include "diagnostic/Diagnostic.hpp"
#include "utils/control.hpp"
#include "utils/style.hpp"

namespace Diagnostic
{
    std::string line_at(Core::ProgramFile &program, size_t line)
    {
        std::stringstream stream(program.source());
        std::string curr_line;

        size_t line_count = 0;
        while (std::getline(stream, curr_line, '\n'))
        {
            line_count++;

            if (line_count == line)
                return curr_line;
        }

        Utils::terminate("Invalid line number provided. Line does not exist!",
                         EXIT_FAILURE);
        return nullptr;
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
        auto &[message, position, length, emphasis, trace, pointer] = options;
        auto &[row, col, program] = position;

        size_t end_pos = node_->end_pos();

        constexpr size_t tab_size = 3;
        std::string tab = Utils::tab(tab_size, 1),
                    row_string = std::to_string(row);

        std::string prefix = tab + row_string + "  | " + tab;
        size_t prefix_len = prefix.size();

        std::string display_line = prefix + line_at(program, row) + " \n";

        display_line.insert(prefix_len + end_pos - 1, Utils::Styles::Reset);
        display_line.insert(prefix_len + col - 1, emphasis);

        display_line.insert(tab_size + row_string.size(), Utils::Styles::Reset);
        display_line.insert(tab_size, Utils::Colors::Yellow);

        constructed_ += display_line;

        std::string offset = Utils::tab((prefix.size() + col) - 1, 1);
        constructed_ +=
            (offset + pointer + "^" + std::string(end_pos - (col + 1), '~') +
             Utils::Styles::Reset + "\n");

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
            detail->report();
    }

} // namespace Diagnostic
