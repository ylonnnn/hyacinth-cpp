#include <iostream>

#include "ast/expr/Expr.hpp"
#include "ast/expr/UnaryExpr.hpp"
#include "lexer/Token.hpp"
#include "utils/style.hpp"

namespace AST
{
    UnaryExpr::UnaryExpr(UnaryType unary_type, Lexer::Token &operation,
                         std::unique_ptr<Expr> expr)
        : Node(unary_type == UnaryType::Pre ? operation.position
                                            : expr->position()),
          unary_type_(unary_type), operation_(operation), expr_(std::move(expr))
    {
        end_pos_ = unary_type_ == UnaryType::Pre
                       ? operation_.position.col + operation_.value.size()
                       : expr->end_pos();
    }

    UnaryType UnaryExpr::unary_type() { return unary_type_; }

    Lexer::Token &UnaryExpr::operation() { return operation_; }

    Expr &UnaryExpr::expr() { return *expr_; }

    void UnaryExpr::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    tabs = Utils::tab(tab, 4);

        os << "UnaryExpr {";

        os << "\n" << tabs << "operation: " << operation_.value;

        os << "\n" << tabs << "expr: ";
        expr_->print(os, tab + 4);

        os << "\n" << indentation << "}";
    }

} // namespace AST
