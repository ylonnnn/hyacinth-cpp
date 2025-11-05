#include "ast/expr/Path.hpp"
#include "utils/style.hpp"

namespace AST
{
    Path::Path(std::vector<std::unique_ptr<Identifier>> &&segments)
        : Node(segments.front()->range.start()), segments(std::move(segments))
    {
        if (!this->segments.empty())
            range.end(this->segments.back()->range.end());
    }

    void Path::add_segment(std::unique_ptr<Identifier> &&segment)
    {
        range.end(segment->range.end());
        segments.push_back(std::move(segment));
    }

    void Path::print(std::ostream &os, uint32_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "Path {\n" << inner_indentation << "segments: {";

        for (const auto &segment : segments)
        {
            std::string inner_indentation = utils::tab(tab + 1, 4);

            os << "\n" << inner_indentation;
            segment->print(os, tab + 2);
        }

        os << "\n" << inner_indentation << "}\n" << indentation << "}";
    }

} // namespace AST
