#include "ast/expr/CollectionExpr.hpp"
#include "ast/expr/Expr.hpp"
#include "core/program/Program.hpp"
#include "utils/style.hpp"

namespace AST
{
    CollectionExpr::CollectionExpr(
        Core::Position position, std::vector<std::unique_ptr<Expr>> collection)
        : NodeCollection<Expr>(std::move(position), std::move(collection))
    {
    }

    void CollectionExpr::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    tabs = Utils::tab(tab, 4);
        os << "CollectionExpr {";

        for (auto &expr : collection_)
            expr->print(os, tab + 1);

        os << "\n" << indentation << "}";
    }

} // namespace AST
