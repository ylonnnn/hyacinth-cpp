#include "ast/expr/Path.hpp"
#include "utils/style.hpp"

namespace AST
{
    Path::Path(std::vector<std::unique_ptr<Identifier>> &&segments)
        : Node(segments.front()->position), segments(std::move(segments))
    {
        if (!this->segments.empty())
            end_position = this->segments.back()->end_position;
    }

    void Path::add_segment(std::unique_ptr<Identifier> &&segment)
    {
        end_position = segment->end_position;
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
