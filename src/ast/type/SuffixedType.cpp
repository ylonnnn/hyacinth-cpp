#include "ast/type/SuffixedType.hpp"
#include "utils/style.hpp"

namespace AST
{
    std::ostream &operator<<(std::ostream &os, SuffixedTypeKind kind)
    {
        switch (kind)
        {
            case SuffixedTypeKind::Optional:
                return os << "Optional";
        }
    }

    SuffixedType::SuffixedType(SuffixedTypeKind kind,
                               std::unique_ptr<Type> &&base)
        : Node(base->position), kind(kind), base(std::move(base))
    {
    }

    std::string SuffixedType::to_string() const
    {
        std::string prefix[] = {"?"}, str = base->to_string();
        str += prefix[static_cast<uint32_t>(kind)];

        return str;
    }

    void SuffixedType::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "SuffixedType {\n"
           << inner_indentation << "kind: " << kind << "\n"
           << inner_indentation << "base: ";

        base->print(os, tab + 1);

        os << "\n" << indentation << "}";
    }

} // namespace AST
