#include "ast/expr/ArrayLiteralExpr.hpp"
#include "ast/expr/Expr.hpp"
#include "utils/style.hpp"

namespace AST
{
    ArrayLiteralExpr::ArrayLiteralExpr(Core::Position &position,
                                       std::unique_ptr<PrefixedType> &&type)
        : Node(position), type(std::move(type))
    {
    }

    void ArrayLiteralExpr::print(std::ostream &os, uint32_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "ArrayLiteralExpr {\n" << inner_indentation << "type: ";

        if (type != nullptr)
            type->print(os, tab + 1);
        else
            os << "<Inferred>";

        os << "\n" << inner_indentation << "elements: {";

        for (auto &expr : elements)
        {
            std::string inner_indentation = utils::tab(tab + 1, 4);

            os << "\n" << inner_indentation;
            expr->print(os, tab + 2);
        }

        os << "\n" << inner_indentation << "\n" << indentation << "}";
    }

} // namespace AST
