#include <cstdint>
#include <iostream>

#include "ast/stmt/ExprStmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    ExprStmt::ExprStmt(std::unique_ptr<AST::Expr> expr)
        : Node(expr->position()), expr_(std::move(expr))
    {
        set_end_position(expr_->end_position());
    }

    Expr &ExprStmt::expr() { return *expr_; }

    std::unique_ptr<Expr> &ExprStmt::expr_ptr() { return expr_; }

    void ExprStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "ExprStmt {"
           << "\n"
           << inner_indentation << "expr: ";
        expr_->print(os, tab + 1);

        os << "\n" << indentation << "}";
    }

} // namespace AST
