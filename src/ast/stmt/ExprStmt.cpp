#include <cstdint>
#include <iostream>

#include "ast/stmt/ExprStmt.hpp"
#include "utils/style.hpp"

namespace AST
{
    ExprStmt::ExprStmt(std::unique_ptr<AST::Expr> expr)
        : Stmt(::Program::Position(expr->position())), expr_(std::move(expr))
    {
    }

    void ExprStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    tabs = Utils::tab(tab, 4);

        os << "ExprStmt {"
           << "\n"
           << tabs << "expr: " << *expr_ << "\n"
           << indentation << "}";
    }

} // namespace AST
