#include "ast/expr/TypeExpr.hpp"
#include "utils/style.hpp"

namespace AST
{
    TypeExpr::TypeExpr(std::unique_ptr<Type> type)
        : Node(type->position()), IdentifierExpr(type->value()),
          type_(std::move(type))
    {
        if (type_ != nullptr)
            set_end_position(type_->end_position());
    }

    Type &TypeExpr::type() { return *type_; }
    std::unique_ptr<Type> &TypeExpr::type_ptr() { return type_; }

    void TypeExpr::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "TypeExpr {\n" << inner_indentation << "type: ";
        type_->print(os, tab + 1);

        os << "\n" << indentation << "}";
    }

} // namespace AST
