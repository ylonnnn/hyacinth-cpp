#include "ast/type/SimpleType.hpp"
#include "utils/style.hpp"

namespace AST
{
    SimpleType::SimpleType(std::unique_ptr<Path> &&base)
        : Node(base->position), base(std::move(base))
    {
        if (this->base != nullptr)
            end_position = this->base->end_position;
    }

    std::string SimpleType::to_string() const
    {
        std::string str;
        for (const auto &segment : base->segments)
        {
            str += segment->identifier.value;

            auto &arguments = segment->arguments;
            if (arguments.size())
                str += "<...>";

            if (segment != base->segments.back())
                str += "::";
        }

        return str;
    }

    void SimpleType::print(std::ostream &os, uint32_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "SimpleType {\n" << inner_indentation << "base: ";

        base->print(os, tab + 1);

        os << "\n" << indentation << "}";
    }

} // namespace AST
