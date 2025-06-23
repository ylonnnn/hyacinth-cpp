#include "ast/expr/CollectionExpr.hpp"
#include "ast/NodeCollection.hpp"
#include "ast/expr/Expr.hpp"
#include "utils/style.hpp"

namespace AST
{
    CollectionExpr::CollectionExpr(
        Program::Position &&position,
        std::vector<std::unique_ptr<Expr>> &&collection)
        : NodeCollection<Expr>(std::move(position), std::move(collection))
    {
    }

    void CollectionExpr::print(std::ostream &os, uint8_t tab) const
    {
        std::string less_indented = Utils::tab(tab - 1), tabs = Utils::tab(tab);
        os << less_indented << "CollectionExpr {";

        for (auto &expr : collection_)
            expr->print(os, tab + 1);

        os << "\n" << less_indented << "}";
    }

    CollectionExpr &CollectionExpr::push_expr(std::unique_ptr<Expr> expr)
    {
        collection_.push_back(std::move(expr));

        return *this;
    }

} // namespace AST
