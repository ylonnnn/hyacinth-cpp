#include "ast/type/PrefixedType.hpp"
#include "utils/style.hpp"

namespace AST
{
    std::ostream &operator<<(std::ostream &os, PrefixedTypeKind kind)
    {
        switch (kind)
        {
            case PrefixedTypeKind::Array:
                return os << "Array";

            case PrefixedTypeKind::Pointer:
                return os << "Pointer";

            case PrefixedTypeKind::Reference:
                return os << "Reference";

            case PrefixedTypeKind::RValueReference:
                return os << "RValueReference";
        }

        return os << "Unknown";
    }

    PrefixedType::PrefixedType(PrefixedTypeKind kind,
                               std::unique_ptr<Type> &&base)
        : Node(base->position), kind(kind), base(std::move(base))
    {
        if (this->base != nullptr)
            end_position = this->base->end_position;
    }

    std::string PrefixedType::to_string() const
    {
        std::string prefix[] = {"[]", "*", "&", "&&"},
                    str = prefix[static_cast<uint32_t>(kind)];

        str += base->to_string();
        return str;
    }

    void PrefixedType::print(std::ostream &os, uint32_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "PrefixedType {\n"
           << inner_indentation << "kind: " << kind << "\n"
           << inner_indentation << "base: ";

        base->print(os, tab + 1);

        os << "\n" << indentation << "}";
    }

} // namespace AST
