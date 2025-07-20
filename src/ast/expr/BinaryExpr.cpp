#include <iostream>

#include "ast/expr/BinaryExpr.hpp"
#include "ast/expr/Expr.hpp"
#include "lexer/Token.hpp"
#include "utils/style.hpp"

namespace AST
{
    BinaryExpr::BinaryExpr(std::unique_ptr<Expr> left, Lexer::Token &operation,
                           std::unique_ptr<Expr> right)
        : Node(left->position()), left_(std::move(left)), operation_(operation),
          right_(std::move(right))
    {
        end_pos_ = right_ != nullptr
                       ? right_->end_pos()
                       : (operation_.position.col + operation_.value.size());
    }

    Expr &BinaryExpr::left() { return *left_; }

    Lexer::Token &BinaryExpr::operation() { return operation_; }

    Expr &BinaryExpr::right() { return *right_; }

    void BinaryExpr::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);
        os << "BinaryExpr {";

        os << "\n" << inner_indentation << "left: ";
        left_->print(os, tab + 1);

        os << "\n" << inner_indentation << "operation: " << operation_.value;

        os << "\n" << inner_indentation << "right: ";
        if (right_ != nullptr)
            right_->print(os, tab + 1);
        else
            os << "nullptr";

        os << "\n" << indentation << "}";
    }

} // namespace AST
