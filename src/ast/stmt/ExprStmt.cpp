#include <cstdint>
#include <iostream>

#include "ast/stmt/ExprStmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    ExprStmt::ExprStmt(std::unique_ptr<AST::Expr> expr)
        : Node(expr->position), expr(std::move(expr))
    {
        end_position = this->expr->end_position;
    }

    void ExprStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "ExprStmt {"
           << "\n"
           << inner_indentation << "expr: ";
        expr->print(os, tab + 1);

        os << "\n" << indentation << "}";
    }

} // namespace AST
