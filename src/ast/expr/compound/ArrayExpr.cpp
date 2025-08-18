#include "ast/expr/compound/ArrayExpr.hpp"
#include "ast/expr/Expr.hpp"
#include "utils/style.hpp"

namespace AST
{
    ArrayExpr::ArrayExpr(Core::Position &position,
                         std::vector<std::unique_ptr<Expr>> elements)
        : Expr::Node(position), elements_(std::move(elements))
    {
    }

    std::vector<std::unique_ptr<Expr>> &ArrayExpr::elements()
    {
        return elements_;
    }

    const std::vector<std::unique_ptr<Expr>> &ArrayExpr::elements() const
    {
        return elements_;
    }

    void ArrayExpr::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "ArrayExpr {\n" << inner_indentation << "elements: {";

        for (auto &expr : elements_)
        {
            std::string inner_indentation = Utils::tab(tab + 1, 4);

            os << "\n" << inner_indentation;
            expr->print(os, tab + 2);
        }

        os << "\n" << inner_indentation << "\n" << indentation << "}";
    }

} // namespace AST
