#include <iostream>

#include "ast/expr/BinaryExpr.hpp"
#include "ast/expr/Expr.hpp"
#include "lexer/Token.hpp"
#include "utils/style.hpp"

namespace AST
{
    BinaryExpr::BinaryExpr(std::unique_ptr<Expr> left, Lexer::Token &operation,
                           std::unique_ptr<Expr> right)
        : Expr(::Program::Position(left->position())), left_(std::move(left)),
          operation_(operation), right_(std::move(right))
    {
        end_pos_ = right_->end_pos();
    }

    Expr &BinaryExpr::left() { return *left_; }

    Lexer::Token &BinaryExpr::operation() { return operation_; }

    Expr &BinaryExpr::right() { return *right_; }

    void BinaryExpr::print(std::ostream &os, uint8_t tab) const
    {
        std::string less_indented = Utils::tab(tab - 1, 4),
                    tabs = Utils::tab(tab, 4);
        os << "BinaryExpr {";

        os << "\n" << tabs << "left: ";
        left_->print(os, tab + 1);

        os << "\n" << tabs << "operation: " << operation_.value;

        os << "\n" << tabs << "right: ";
        right_->print(os, tab + 1);

        os << "\n" << less_indented << "}";
    }

} // namespace AST
