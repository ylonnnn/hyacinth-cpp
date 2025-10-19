#include "ast/type/ModifiedType.hpp"
#include "utils/style.hpp"

namespace AST
{
    std::ostream &operator<<(std::ostream &os, ModifierType type)
    {
        switch (type)
        {
            case ModifierType::Mutable:
                return os << "Mutable";
        }

        return os << "Unknown";
    }

    ModifiedType::ModifiedType(ModifierType type, std::unique_ptr<Type> &&base)
        : Node(base->position), type(type), base(std::move(base))
    {
    }

    std::string ModifiedType::to_string() const
    {
        std::string modifier[] = {"mut"},
                    str = modifier[static_cast<uint32_t>(type)];

        str += base->to_string();
        return str;
    }

    void ModifiedType::print(std::ostream &os, uint32_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "ModifiedType {\n"
           << inner_indentation << "type: " << type << "\n"
           << inner_indentation << "base: ";

        base->print(os, tab + 1);

        os << "\n" << indentation << "}";
    }

} // namespace AST
